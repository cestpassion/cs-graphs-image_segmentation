#ifndef FELZENSZWALB_SEGMENTER_HPP
#define FELZENSZWALB_SEGMENTER_HPP

#include <vector>
#include "Graph.hpp"

// Função principal que executa o método de Felzenszwalb e Huttenlocher. (nome grandinho hein)
// Retorna um vetor onde o índice é o ID do pixel e o valor é a label do segmento.
std::vector<int> segmentFelzenszwalb(Graph& graph, double k, int min_size);

#endif