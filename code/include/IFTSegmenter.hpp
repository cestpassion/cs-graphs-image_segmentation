#ifndef IFT_SEGMENTER_HPP
#define IFT_SEGMENTER_HPP

#include <string>
#include <vector>

#include "Graph.hpp"
#include "Image.hpp"

struct Seed {
    int x;
    int y;
    int label;
};

/**
 * Resultado completo da IFT, com os tres mapas que o enunciado pede armazenar
 * para cada pixel.
 */
struct IFTResult {
    std::vector<double> cost;         
    std::vector<int> predecessor;     // -1 = raiz/sem
    std::vector<int> label;           
};

IFTResult runIFT(const Graph& graph, const std::vector<Seed>& seeds);

std::vector<int> segmentIFT(const Graph& graph, const std::vector<Seed>& seeds);

std::vector<Seed> loadSeedsFromFile(const std::string& path, int width,
                                    int height, bool& ok);

std::vector<Seed> generateAutomaticSeeds(int width, int height, int rows,
                                         int cols);


std::vector<Seed> seedsFromCoordinateList(const std::vector<int>& values,
                                          int width, int height, bool& ok);
                                          
// Exporta a matriz de custos como imagem
Image createIFTCostImage(const IFTResult& result, int width, int height);

#endif