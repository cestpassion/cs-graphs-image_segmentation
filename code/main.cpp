#include "CLI.hpp"
#include "CoustySegmenter.hpp"
#include "FelzenszwalbSegmenter.hpp"
#include "IFTSegmenter.hpp"
#include "Image.hpp"
#include "Graph.hpp"
#include "MST.hpp"

#include <iostream>
#include <filesystem>
#include <functional>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;

void processSingleImage(std::string inputPath, std::string outPrefix, const CLIOptions& options) {
    Image orgImage;
    if (!orgImage.load(inputPath)) {
        std::cerr << "Erro ao carregar: " << inputPath << std::endl;
        return;
    }

    Image procImage = options.useMedian ? orgImage.applyMedianFilter() : orgImage;
    if (options.gray) {
        procImage = procImage.toGrayscale();
    }

    if (options.method == "copy") {
        if (!procImage.save(outPrefix + "_copy.png")) {
            std::cerr << "Erro I/O: Nao foi possivel salvar imagem de copia: " << outPrefix << std::endl;
        } else {
            std::cout << "Sucesso no processamento de (copy): " << outPrefix << std::endl;
        }
        return;
    }

    Connectivity conn = (options.neighborhood == 8) ? Connectivity::EIGHT : Connectivity::FOUR;
    auto weightFunc = [&procImage](int u, int v) -> double {
        auto [xu, yu] = idToCoord(u, procImage.width);
        auto [xv, yv] = idToCoord(v, procImage.width);
        if (procImage.channels == 1) {
            return grayWeight(procImage.getPixel(xu, yu, 0), procImage.getPixel(xv, yv, 0));
        }
        return colorWeight(
            procImage.getPixel(xu, yu, 0), procImage.getPixel(xu, yu, 1), procImage.getPixel(xu, yu, 2),
            procImage.getPixel(xv, yv, 0), procImage.getPixel(xv, yv, 1), procImage.getPixel(xv, yv, 2)
        );
    };
    
    Graph graph = buildGraph(procImage.width, procImage.height, conn, weightFunc);
    std::vector<int> labels;

    if (options.method == "felzenszwalb") {
        labels = segmentFelzenszwalb(graph, options.k, options.minSize);
    } 
    else if (options.method == "cousty") {
        MST mst = buildMST(graph);
        
        std::vector<int> thresholds = options.thresholds;
        if (thresholds.empty()) thresholds.push_back(options.threshold);

        for (int t : thresholds) {
            labels = segmentCoustyByThreshold(mst, t);
            
            std::string t_suffix = (thresholds.size() > 1) ? ("_t" + std::to_string(t)) : "";
            
            Image randImg = Image::createColoredLabelImage(labels, procImage.width, procImage.height);
            Image avgImg = Image::createAvgColorLabelImage(labels, orgImage);
            Image boundImg = Image::createBoundaryLabelImage(labels, orgImage);

            bool ok = true;
            ok &= randImg.save(outPrefix + "_cousty" + t_suffix + "_random.png");
            ok &= avgImg.save(outPrefix + "_cousty" + t_suffix + "_avg.png");
            ok &= boundImg.save(outPrefix + "_cousty" + t_suffix + "_boundary.png");

            if (!ok) {
                std::cerr << "Erro I/O: Falha ao salvar as imagens Cousty para limiar " << t << std::endl;
            }
        }
        
        Image saliency = createCoustySaliencyImage(mst);
        if (!saliency.save(outPrefix + "_cousty_saliency.png")) {
            std::cerr << "Erro I/O: Falha ao salvar mapa de saliencia Cousty." << std::endl;
        } else {
            std::cout << "Sucesso no processamento de: " << outPrefix << std::endl;
        }
        return; 
    } 
    else if (options.method == "ift") {
        bool ok = true;
        std::vector<Seed> seeds;
        if (!options.seedsPath.empty()) {
            seeds = loadSeedsFromFile(options.seedsPath, procImage.width, procImage.height, ok);
        } else if (options.autoSeeds) {
            seeds = generateAutomaticSeeds(procImage.width, procImage.height, options.autoSeedRows, options.autoSeedCols);
        } else if (!options.manualSeeds.empty()){
            seeds = seedsFromCoordinateList(options.manualSeeds, procImage.width, procImage.height, ok);
        } else {
            ok = false;
        }
        
        if (!ok || seeds.empty()) {
            std::cerr << "Erro: Nenhuma semente valida fornecida para o metodo IFT em " << inputPath << std::endl;
            return;
        }
        
        IFTResult iftRes = runIFT(graph, seeds);
        labels = iftRes.label;
        Image costImg = createIFTCostImage(iftRes, procImage.width, procImage.height);
        if (!costImg.save(outPrefix + "_ift_cost.png")) {
            std::cerr << "Erro I/O: Falha ao salvar mapa de custos do IFT." << std::endl;
        }
    } else {
        std::cerr << "Erro: Metodo invalido '" << options.method << "'." << std::endl;
        return;
    }

    if (!labels.empty()) {
        Image randImg = Image::createColoredLabelImage(labels, procImage.width, procImage.height);
        Image avgImg = Image::createAvgColorLabelImage(labels, orgImage);
        Image boundImg = Image::createBoundaryLabelImage(labels, orgImage);
        
        bool ok = true;
        ok &= randImg.save(outPrefix + "_" + options.method + "_random.png");
        ok &= avgImg.save(outPrefix + "_" + options.method + "_avg.png");
        ok &= boundImg.save(outPrefix + "_" + options.method + "_boundary.png");

        if (!ok) {
            std::cerr << "Erro I/O: Falha ao salvar as imagens processadas finais para " << outPrefix << std::endl;
        } else {
            std::cout << "Sucesso no processamento de: " << outPrefix << std::endl;
        }
    } else {
        std::cerr << "Erro: A segmentacao nao gerou regioes validas para " << inputPath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    CLIOptions options = CLI::parse(argc, argv);
    
    if (options.help || !options.valid) {
        CLI::printHelp();
        if (!options.errorMessage.empty()) std::cerr << "Erro: " << options.errorMessage << std::endl;
        return options.help ? 0 : 1;
    }

    if (options.batchMode) {
        try {
            if (!fs::exists(options.inputPath) || !fs::is_directory(options.inputPath)) {
                std::cerr << "Erro: O argumento --input '" << options.inputPath << "' nao e um diretorio valido.\n";
                return 1;
            }
            fs::create_directories(options.outputPath);
            
            for (const auto& entry : fs::directory_iterator(options.inputPath)) {
                if (entry.is_regular_file()) {
                    std::string ext = entry.path().extension().string();
                    if (ext == ".jpg" || ext == ".jpeg" || ext == ".png") {
                        std::string filename = entry.path().stem().string();
                        std::string outPrefix = options.outputPath + "/" + filename;
                        processSingleImage(entry.path().string(), outPrefix, options);
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Erro critico no sistema de arquivos durante modo batch: " << e.what() << std::endl;
            return 1;
        }
    } else {
        try {
            fs::path outPath(options.outputPath);
            fs::path parent = outPath.parent_path();
            if (!parent.empty()) {
                fs::create_directories(parent);
            }
            
            std::string outPrefix = parent.string();
            if (!outPrefix.empty()) outPrefix += "/";
            outPrefix += outPath.stem().string();
            
            processSingleImage(options.inputPath, outPrefix, options);
        } catch (const std::exception& e) {
            std::cerr << "Erro critico no sistema de arquivos: " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}