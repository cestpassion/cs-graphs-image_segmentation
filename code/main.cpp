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

    // Tratamento direto para o método copy
    if (options.method == "copy") {
        procImage.save(outPrefix + "_copy.png");
        std::cout << "Sucesso no processamento de (copy): " << outPrefix << std::endl;
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
            
            // Se tiver múltiplos thresholds, salva identificando-os
            std::string t_suffix = (thresholds.size() > 1) ? ("_t" + std::to_string(t)) : "";
            
            Image randImg = Image::createColoredLabelImage(labels, procImage.width, procImage.height);
            randImg.save(outPrefix + "_cousty" + t_suffix + "_random.png");

            Image avgImg = Image::createAvgColorLabelImage(labels, orgImage);
            avgImg.save(outPrefix + "_cousty" + t_suffix + "_avg.png");

            Image boundImg = Image::createBoundaryLabelImage(labels, orgImage);
            boundImg.save(outPrefix + "_cousty" + t_suffix + "_boundary.png");
        }
        
        Image saliency = createCoustySaliencyImage(mst);
        saliency.save(outPrefix + "_cousty_saliency.png");
        std::cout << "Sucesso no processamento de: " << outPrefix << std::endl;
        return; // Retorna pois o Cousty já processou as múltiplas saídas no laço
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
        costImg.save(outPrefix + "_ift_cost.png");
    } else {
        std::cerr << "Erro: Metodo invalido '" << options.method << "'." << std::endl;
        return;
    }

    if (!labels.empty()) {
        Image randImg = Image::createColoredLabelImage(labels, procImage.width, procImage.height);
        randImg.save(outPrefix + "_" + options.method + "_random.png");

        Image avgImg = Image::createAvgColorLabelImage(labels, orgImage);
        avgImg.save(outPrefix + "_" + options.method + "_avg.png");

        Image boundImg = Image::createBoundaryLabelImage(labels, orgImage);
        boundImg.save(outPrefix + "_" + options.method + "_boundary.png");
        
        std::cout << "Sucesso no processamento de: " << outPrefix << std::endl;
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
    } else {
        fs::path outPath(options.outputPath);
        fs::path parent = outPath.parent_path();
        if (!parent.empty()) {
            fs::create_directories(parent);
        }
        
        std::string outPrefix = parent.string();
        if (!outPrefix.empty()) outPrefix += "/";
        outPrefix += outPath.stem().string();
        
        processSingleImage(options.inputPath, outPrefix, options);
    }

    return 0;
}