#ifndef COUSTY_SEGMENTER_HPP
#define COUSTY_SEGMENTER_HPP

#include <vector>

#include "Graph.hpp"
#include "Image.hpp"
#include "MST.hpp"

std::vector<int> segmentCoustyByThreshold(const MST& mst, double threshold);

std::vector<int> segmentCousty(const Graph& graph, double threshold);

Image createCoustySaliencyImage(const MST& mst);

#endif  // COUSTY_SEGMENTER_HPP