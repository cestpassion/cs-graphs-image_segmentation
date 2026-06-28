#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <functional>
#include <utility>
#include <vector>

#include "Edge.hpp"

/**
 * Grafo nao direcionado e ponderado que representa uma imagem digital.
 *
 * Cada pixel e um vertice (id = y * width + x; origem (0,0) no canto superior
 * esquerdo, x cresce para a direita, y para baixo). Cada aresta liga pixels
 * vizinhos e existe uma unica vez (sem duplicata {v,u}). Esta e apenas a camada
 * de representacao: nao contem nenhum algoritmo de segmentacao.
 */
struct Graph {
    int numVertices;          // = width * height
    std::vector<Edge> edges;  // arestas unicas (cada {u,v} criada uma so vez)
    int width;                // largura da imagem, em pixels
    int height;               // altura da imagem, em pixels
};

/** Tipo de vizinhanca usado na construcao do grafo. */
enum class Connectivity { FOUR, EIGHT };

/** Retorna o id do vertice na posicao (x, y): id = y * width + x. */
int coordToId(int x, int y, int width);

/** Inversa de coordToId: devolve {x, y} a partir do id. */
std::pair<int, int> idToCoord(int id, int width);

/** Tons de cinza: diferenca absoluta de intensidade |i1 - i2|. */
double grayWeight(int i1, int i2);

/** Cor (RGB): distancia euclidiana sqrt(dr^2 + dg^2 + db^2). */
double colorWeight(int r1, int g1, int b1, int r2, int g2, int b2);

/**
 * Monta o grafo de uma imagem width x height, sem arestas duplicadas.
 *
 * connectivity escolhe vizinhanca 4 (direita/baixo) ou 8 (+ diagonais).
 * weight(u, v) recebe os ids dos dois pixels de uma aresta e devolve a
 * dissimilaridade; o chamador "fecha" sobre os dados de pixel (cinza ou cor),
 * mantendo esta camada independente do tipo de imagem e de qualquer algoritmo
 * de segmentacao.
 */
Graph buildGraph(int width, int height, Connectivity connectivity,
                 const std::function<double(int u, int v)>& weight);

#endif  // GRAPH_HPP
