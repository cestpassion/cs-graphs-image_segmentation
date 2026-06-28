#include "Graph.hpp"
#include <cmath>

// Convencao: origem (0,0) no canto superior esquerdo; x cresce para a
// direita, y para baixo. O id percorre a imagem linha a linha.
int coordToId(int x, int y, int width)
{
    return y * width + x; // É a fórmula da convenção da issue
}

// Inversa de coordToId.
//   x = id % width   (quanto sobra dentro da linha)
//   y = id / width   (qual linha, divisao inteira)
std::pair<int, int> idToCoord(int id, int width)
{
    return {id % width, id / width};
}

// Peso para tons de cinza: o quanto as duas intensidades diferem.
// Pixels iguais dao peso 0; quanto mais diferentes, maior o peso.
double grayWeight(int i1, int i2)
{
    return std::abs(i1 - i2);
}

// Peso para cor (RGB): a "distancia" entre as duas cores.
// Olha a diferenca em cada canal (R, G, B) e junta tudo como uma distancia em linha reta. Mesma cor da 0; cores bem diferentes dao mais.
double colorWeight(int r1, int g1, int b1, int r2, int g2, int b2)
{
    int dr = r1 - r2;
    int dg = g1 - g2;
    int db = b1 - b2;
    return std::sqrt(static_cast<double>(dr * dr + dg * dg + db * db));
}

// Monta o grafo da imagem: um vertice por pixel e arestas ligando vizinhos.
// Truque para nao repetir aresta: cada pixel so cria ligacao para os vizinhos
// "a frente" dele (a direita e para baixo). Como todos os pixels passam por
// aqui, cada par de vizinhos acaba ligado exatamente uma vez.
// O peso de cada aresta vem da funcao "weight", que o chamador escolhe
// (cinza ou cor) - assim esta parte nao precisa saber o tipo da imagem.
Graph buildGraph(int width, int height, Connectivity connectivity,
                 const std::function<double(int u, int v)>& weight)
{
    Graph graph;
    graph.width = width;
    graph.height = height;
    graph.numVertices = width * height;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int u = coordToId(x, y, width);

            // vizinho a direita
            if (x + 1 < width)
            {
                int v = coordToId(x + 1, y, width);
                graph.edges.push_back({u, v, weight(u, v)});
            }

            // vizinho abaixo
            if (y + 1 < height)
            {
                int v = coordToId(x, y + 1, width);
                graph.edges.push_back({u, v, weight(u, v)});
            }

            // as duas diagonais de baixo so existem na vizinhanca 8
            if (connectivity == Connectivity::EIGHT)
            {
                // diagonal de baixo a direita
                if (x + 1 < width && y + 1 < height)
                {
                    int v = coordToId(x + 1, y + 1, width);
                    graph.edges.push_back({u, v, weight(u, v)});
                }

                // diagonal de baixo a esquerda
                if (x - 1 >= 0 && y + 1 < height)
                {
                    int v = coordToId(x - 1, y + 1, width);
                    graph.edges.push_back({u, v, weight(u, v)});
                }
            }
        }
    }

    return graph;
}
