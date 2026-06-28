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

namespace fs = std::filesystem;

void processSingleImage(std::string inputPath, std::string outputDir, const CLIOptions& options) {
    Image orgImage;
    if (!orgImage.load(inputPath)) {
        std::cerr << "Erro ao carregar: " << inputPath << std::endl;
        return;
    }

    std::string filename = fs::path(inputPath).stem().string();
    fs::create_directories(outputDir);

    Image procImage = options.useMedian ? orgImage.applyMedianFilter() : orgImage;
    if (options.gray) {
        procImage = procImage.toGrayscale();
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
        labels = segmentCoustyByThreshold(mst, options.threshold);
        Image saliency = createCoustySaliencyImage(mst);
        saliency.save(outputDir + "/" + filename + "_cousty_saliency.png");
    } 
    else if (options.method == "ift") {
        bool ok = false;
        std::vector<Seed> seeds;
        if (!options.seedsPath.empty()) {
            seeds = loadSeedsFromFile(options.seedsPath, procImage.width, procImage.height, ok);
        } else if (options.autoSeeds) {
            seeds = generateAutomaticSeeds(procImage.width, procImage.height, options.autoSeedRows, options.autoSeedCols);
        } else {
            seeds = seedsFromCoordinateList(options.manualSeeds, procImage.width, procImage.height, ok);
        }
        
        IFTResult iftRes = runIFT(graph, seeds);
        labels = iftRes.label;
        Image costImg = createIFTCostImage(iftRes, procImage.width, procImage.height);
        costImg.save(outputDir + "/" + filename + "_ift_cost.png");
    }

    if (!labels.empty()) {
        Image randImg = Image::createColoredLabelImage(labels, procImage.width, procImage.height);
        randImg.save(outputDir + "/" + filename + "_" + options.method + "_random.png");

        Image avgImg = Image::createAvgColorLabelImage(labels, orgImage);
        avgImg.save(outputDir + "/" + filename + "_" + options.method + "_avg.png");

        Image boundImg = Image::createBoundaryLabelImage(labels, orgImage);
        boundImg.save(outputDir + "/" + filename + "_" + options.method + "_boundary.png");
        
        std::cout << "Sucesso no processamento de: " << filename << " -> " << outputDir << std::endl;
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
        std::string inputDir = options.inputPath;
        if (!fs::is_directory(inputDir)) {
            std::cerr << "Para modo batch, --input deve indicar uma pasta valida (ex: data/input/)" << std::endl;
            return 1;
        }
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".jpg" || ext == ".jpeg" || ext == ".png") {
                    processSingleImage(entry.path().string(), options.outputPath, options);
                }
            }
        }
    } else {
        processSingleImage(options.inputPath, fs::path(options.outputPath).parent_path().string(), options);
    }

    return 0;
}