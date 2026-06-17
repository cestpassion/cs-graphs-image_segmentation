#ifndef MST_HPP
#define MST_HPP

#include <vector>

#include "Edge.hpp"
#include "Graph.hpp"

struct MST {
    // A MST vai guardar apenas as arestas escolhidas no método buildMST() usando Kruskal
    // com isso, vai dar para reconstruir os cortes hierárquicos depois, sem manter o grafo original.
    int numVertices = 0;
    int width = 0;
    int height = 0;
    std::vector<Edge> edges;
};


// O método usando para buildar a MST é Kruskal, já que o grafo da imagem já chega com as arestas ponderadas
// e com o UnionFind reduz o custo de detectar ciclos.
MST buildMST(const Graph& graph);

#endif  // MST_HPP