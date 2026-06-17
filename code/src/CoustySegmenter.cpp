#include "CoustySegmenter.hpp"

#include "UnionFind.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_map>

namespace {

std::vector<int> relabelComponents(UnionFind& uf, int numVertices) {
    // Depois do corte da MST, cada raiz do UnionFind representa um segmento;
    // aqui normalizamos essas raízes para labels compactos 0..N-1.
    std::unordered_map<int, int> labels;
    std::vector<int> result(numVertices, 0);
    int nextLabel = 0;

    for (int vertex = 0; vertex < numVertices; ++vertex) {
        int root = uf.find(vertex);
        auto [it, inserted] = labels.emplace(root, nextLabel);
        if (inserted) {
            ++nextLabel;
        }
        result[vertex] = it->second;
    }

    return result;
}

Image buildSaliencyImage(const MST& mst) {
    Image saliency;
    saliency.width = mst.width;
    saliency.height = mst.height;
    saliency.channels = 1;

    // O mapa de saliência aqui é uma visualização simplificada: um pixel fica
    // mais claro quando participa de arestas mais pesadas da MST.
    std::size_t totalPixels = static_cast<std::size_t>(mst.width) * mst.height;
    saliency.pixels.assign(totalPixels, 0);

    if (totalPixels == 0 || mst.edges.empty()) {
        return saliency;
    }

    std::vector<double> vertexStrength(totalPixels, 0.0);
    double maxWeight = 0.0;

    for (const Edge& edge : mst.edges) {
        maxWeight = std::max(maxWeight, edge.weight);
        vertexStrength[static_cast<std::size_t>(edge.src)] =
            std::max(vertexStrength[static_cast<std::size_t>(edge.src)], edge.weight);
        vertexStrength[static_cast<std::size_t>(edge.dest)] =
            std::max(vertexStrength[static_cast<std::size_t>(edge.dest)], edge.weight);
    }

    for (std::size_t i = 0; i < totalPixels; ++i) {
        double normalized = maxWeight > 0.0 ? vertexStrength[i] / maxWeight : 0.0;
        normalized = std::clamp(normalized, 0.0, 1.0);
        saliency.pixels[i] = static_cast<unsigned char>(std::lround(normalized * 255.0));
    }

    return saliency;
}

}  // namespace

std::vector<int> segmentCoustyByThreshold(const MST& mst, double threshold) {
    UnionFind uf(mst.numVertices);

    // O corte hierárquico é propositalmente simples: arestas acima do limiar
    // viram fronteiras e as restantes mantêm os componentes conectados.
    for (const Edge& edge : mst.edges) {
        if (edge.weight <= threshold) {
            uf.unite(edge.src, edge.dest);
        }
    }

    return relabelComponents(uf, mst.numVertices);
}

std::vector<int> segmentCousty(const Graph& graph, double threshold) {
    // Esta sobrecarga evita que o chamador precise conhecer a etapa interna
    // de construção da MST quando só quer uma segmentação por limiar.
    MST mst = buildMST(graph);
    return segmentCoustyByThreshold(mst, threshold);
}

Image createCoustySaliencyImage(const MST& mst) {
    return buildSaliencyImage(mst);
}