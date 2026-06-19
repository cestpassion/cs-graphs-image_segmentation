#include "IFTSegmenter.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>

#include "Graph.hpp"

// Grafo artificial 4x2 com uma borda forte separando a metade esquerda
// (x < 2) da direita (x >= 2). Mesma ideia usada nos testes de Cousty, para
// que o comportamento da IFT possa ser comparado ao dos metodos por MST.
static Graph buildTwoRegionGraph() {
    auto weightFunc = [](int u, int v) {
        int xu = u % 4;
        int xv = v % 4;
        bool leftU = xu < 2;
        bool leftV = xv < 2;
        if (leftU != leftV) {
            return 200.0;  // fronteira forte entre as metades
        }
        return 0.0;  // interior homogeneo
    };
    return buildGraph(4, 2, Connectivity::FOUR, weightFunc);
}

// Criterio de aceite: cada pixel deve receber um label ao final, e a IFT deve
// aceitar multiplas sementes.
static void testEveryPixelLabeled() {
    Graph graph = buildTwoRegionGraph();
    std::vector<Seed> seeds = {{0, 0, 1}, {3, 0, 2}};  // uma em cada metade

    IFTResult result = runIFT(graph, seeds);

    assert(static_cast<int>(result.label.size()) == graph.numVertices);
    assert(static_cast<int>(result.cost.size()) == graph.numVertices);
    assert(static_cast<int>(result.predecessor.size()) == graph.numVertices);

    for (int label : result.label) {
        assert(label == 1 || label == 2);
    }

    assert(result.label[coordToId(0, 0, 4)] == 1);
    assert(result.label[coordToId(1, 1, 4)] == 1);
    assert(result.label[coordToId(2, 0, 4)] == 2);
    assert(result.label[coordToId(3, 1, 4)] == 2);
}

static void testResultChangesWithSeeds() {
    Graph graph = buildTwoRegionGraph();

    std::vector<int> a = segmentIFT(graph, {{0, 0, 1}, {3, 0, 2}});
    std::vector<int> b = segmentIFT(graph, {{0, 0, 7}}); 

    std::set<int> labelsB(b.begin(), b.end());
    assert(labelsB.size() == 1);
    assert(*labelsB.begin() == 7);

    std::set<int> labelsA(a.begin(), a.end());
    assert(labelsA.size() == 2);
    assert(a != b);
}

// verific a funcao de custo fmax: cost(q) = max(cost(p), peso(p,q)).
// numa linha 1x3 com pesos 5 e 9 a partir da semente na ponta, o custo do
// ultimo pixel deve ser o maior peso do caminho (9), nao a soma.
static void testFmaxCost() {
    auto weightFunc = [](int u, int v) {
        int a = std::min(u, v);
        int b = std::max(u, v);
        if (a == 0 && b == 1) return 5.0;
        if (a == 1 && b == 2) return 9.0;
        return 0.0;
    };
    Graph graph = buildGraph(3, 1, Connectivity::FOUR, weightFunc);

    IFTResult result = runIFT(graph, {{0, 0, 1}});

    assert(std::fabs(result.cost[0] - 0.0) < 1e-9);  
    assert(std::fabs(result.cost[1] - 5.0) < 1e-9);  
    assert(std::fabs(result.cost[2] - 9.0) < 1e-9);  

    assert(result.predecessor[0] == -1);
    assert(result.predecessor[1] == 0);
    assert(result.predecessor[2] == 1);
}

static void testNoSeeds() {
    Graph graph = buildTwoRegionGraph();
    IFTResult result = runIFT(graph, {});

    for (int label : result.label) {
        assert(label == 0);
    }
    for (int pred : result.predecessor) {
        assert(pred == -1);
    }
}

static void testAutomaticSeeds() {
    std::vector<Seed> seeds = generateAutomaticSeeds(100, 80, 2, 3);
    assert(seeds.size() == 6);

    std::set<int> labels;
    for (const Seed& seed : seeds) {
        assert(seed.x >= 0 && seed.x < 100);
        assert(seed.y >= 0 && seed.y < 80);
        labels.insert(seed.label);
    }
    assert(labels.size() == 6);  
}

static void testCoordinateListSeeds() {
    bool ok = false;
    std::vector<int> values = {0, 0, 1, 3, 1, 2};
    std::vector<Seed> seeds = seedsFromCoordinateList(values, 4, 2, ok);

    assert(ok);
    assert(seeds.size() == 2);
    assert(seeds[0].x == 0 && seeds[0].y == 0 && seeds[0].label == 1);
    assert(seeds[1].x == 3 && seeds[1].y == 1 && seeds[1].label == 2);

    bool ok2 = true;
    std::vector<int> bad = {0, 0, 1, 3};
    seedsFromCoordinateList(bad, 4, 2, ok2);
    assert(!ok2);
}

static void testFileSeeds() {
    const std::string path = "tmp_seeds_test.txt";
    {
        std::ofstream out(path);
        out << "# arquivo de sementes de teste\n";
        out << "\n";
        out << "0 0 1\n";
        out << "3 1 2\n";
        out << "999 999 3\n";  // fora dos limites: ignorada
    }

    bool ok = false;
    std::vector<Seed> seeds = loadSeedsFromFile(path, 4, 2, ok);

    assert(ok);
    assert(seeds.size() == 2);
    assert(seeds[0].label == 1);
    assert(seeds[1].label == 2);

    std::remove(path.c_str());

    bool ok2 = true;
    loadSeedsFromFile("arquivo_que_nao_existe_12345.txt", 4, 2, ok2);
    assert(!ok2);
}

int main() {
    testEveryPixelLabeled();
    testResultChangesWithSeeds();
    testFmaxCost();
    testNoSeeds();
    testAutomaticSeeds();
    testCoordinateListSeeds();
    testFileSeeds();
    std::cout << "Todos os testes de IFT passaram com sucesso!" << std::endl;
    return 0;
}
