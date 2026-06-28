#include "MST.hpp"

#include "UnionFind.hpp"

#include <algorithm>

MST buildMST(const Graph& graph) {
    MST mst;
    mst.numVertices = graph.numVertices;
    mst.width = graph.width;
    mst.height = graph.height;

    if (graph.numVertices <= 0) {
        return mst;
    }

    // A ordenação crescente garante que Kruskal privilegie conexões mais
    // parecidas primeiro, o que é coerente com segmentação baseada em bordas.
    std::vector<Edge> sortedEdges = graph.edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(), [](const Edge& a, const Edge& b) {
        if (a.weight != b.weight) {
            return a.weight < b.weight;
        }
        if (a.src != b.src) {
            return a.src < b.src;
        }
        return a.dest < b.dest;
    });

    UnionFind uf(graph.numVertices);
    mst.edges.reserve(graph.numVertices > 0 ? graph.numVertices - 1 : 0);

    for (const Edge& edge : sortedEdges) {
        // Ignoramos arestas que fechariam ciclo; MST, por definição, mantém
        // a conectividade com o menor custo total possível.
        if (uf.find(edge.src) == uf.find(edge.dest)) {
            continue;
        }

        uf.unite(edge.src, edge.dest);
        mst.edges.push_back(edge);

        if (static_cast<int>(mst.edges.size()) == graph.numVertices - 1) {
            break;
        }
    }

    return mst;
}