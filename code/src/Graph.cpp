#include "Graph.hpp"

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
