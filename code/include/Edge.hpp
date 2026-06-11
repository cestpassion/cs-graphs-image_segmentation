#ifndef EDGE_HPP
#define EDGE_HPP

/**
 * Aresta nao direcionada e ponderada do grafo de imagem.
 *
 * src e dest sao ids de vertice (convencao: id = y * width + x).
 * weight e a dissimilaridade entre os dois pixels ligados pela aresta:
 *   - vale 0 quando os pixels sao identicos;
 *   - cresce quanto mais diferentes eles forem (fronteiras = pesos altos).
 */
struct Edge {
    int src;        // vertice de origem
    int dest;       // vertice de destino
    double weight;  // peso (dissimilaridade) da aresta
};

#endif  // EDGE_HPP
