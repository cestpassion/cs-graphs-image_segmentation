#include "CoustySegmenter.hpp"

#include <cassert>
#include <iostream>
#include <set>

static Graph buildTwoRegionGraph() {
    // O grafo artificial separa a imagem em duas metades com uma borda forte
    // para verificar se o threshold realmente controla o número de segmentos.
    auto weightFunc = [](int u, int v) {
        int xu = u % 4;
        int xv = v % 4;
        bool leftU = xu < 2;
        bool leftV = xv < 2;

        if (leftU != leftV) {
            return 200.0;
        }

        return 0.0;
    };

    return buildGraph(4, 2, Connectivity::FOUR, weightFunc);
}

static void testMSTSize() {
    // Uma MST de um grafo conexo com V vértices precisa ter V-1 arestas.
    Graph graph = buildTwoRegionGraph();
    MST mst = buildMST(graph);

    assert(mst.numVertices == graph.numVertices);
    assert(mst.edges.size() == static_cast<std::size_t>(graph.numVertices - 1));
}

static void testThresholdChangesSegmentation() {
    // Threshold baixo mantém a fronteira forte, threshold alto colapsa tudo em
    // um segmento. Esse é o comportamento mínimo esperado para a hierarquia.
    Graph graph = buildTwoRegionGraph();

    std::vector<int> low = segmentCoustyByThreshold(buildMST(graph), 10.0);
    std::vector<int> high = segmentCoustyByThreshold(buildMST(graph), 250.0);

    std::set<int> lowLabels(low.begin(), low.end());
    std::set<int> highLabels(high.begin(), high.end());

    assert(lowLabels.size() > highLabels.size());
    assert(highLabels.size() == 1);
}

static void testSaliencyImage() {
    // A imagem de saliência existe para dar suporte visual à leitura da MST.
    Graph graph = buildTwoRegionGraph();
    MST mst = buildMST(graph);
    Image saliency = createCoustySaliencyImage(mst);

    assert(!saliency.empty());
    assert(saliency.width == graph.width);
    assert(saliency.height == graph.height);
    assert(saliency.channels == 1);
}

int main() {
    testMSTSize();
    testThresholdChangesSegmentation();
    testSaliencyImage();
    std::cout << "Todos os testes de Cousty passaram com sucesso!" << std::endl;
    return 0;
}