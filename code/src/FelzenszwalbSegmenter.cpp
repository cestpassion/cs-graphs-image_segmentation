#include "FelzenszwalbSegmenter.hpp"
#include "UnionFind.hpp"
#include <algorithm>

std::vector<int> segmentFelzenszwalb(Graph& graph, double k, int min_size) {
    // 1. Ordenar as arestas por peso crescente
    // Isso é equivalente a ordenar uma lista de custos de pathfinding do menor para o maior
    std::sort(graph.edges.begin(), graph.edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // 2. Inicializar as estruturas
    UnionFind uf(graph.numVertices);
    
    // O threshold inicial de cada vértice isolado é apenas k (pois tamanho = 1)
    std::vector<double> thresholds(graph.numVertices, k);

    // 3. Primeira Passagem: Construir a floresta baseando-se na similaridade
    for (const Edge& edge : graph.edges) {
        int rootSrc = uf.find(edge.src);
        int rootDest = uf.find(edge.dest);

        // Se eles pertencem a componentes diferentes, avaliamos a fusão
        if (rootSrc != rootDest) {
            // A condição de união: o peso da aresta deve ser menor que o limite de ambos
            if (edge.weight <= thresholds[rootSrc] && edge.weight <= thresholds[rootDest]) {
                uf.unite(rootSrc, rootDest);
                
                // Encontrar o novo "pai" do componente fundido
                int newRoot = uf.find(rootSrc);
                
                // Atualizar o limite interno (Int) do novo componente somado com a tolerância tau
                thresholds[newRoot] = edge.weight + (k / uf.getSize(newRoot));
            }
        }
    }

    // 4. Segunda Passagem: Forçar a fusão de componentes muito pequenos (min_size)
    // Isso limpa os "ruídos" na imagem
    for (const Edge& edge : graph.edges) {
        int rootSrc = uf.find(edge.src);
        int rootDest = uf.find(edge.dest);

        if (rootSrc != rootDest) {
            if (uf.getSize(rootSrc) < min_size || uf.getSize(rootDest) < min_size) {
                uf.unite(rootSrc, rootDest);
            }
        }
    }

    // 5. Gerar a saída: mapear cada pixel para a sua label final
    std::vector<int> labels(graph.numVertices);
    for (int i = 0; i < graph.numVertices; ++i) {
        labels[i] = uf.find(i);
    }

    return labels;
}