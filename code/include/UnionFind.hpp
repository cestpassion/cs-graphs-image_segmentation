#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<int> size; // Necessário para a fórmula matemática do Felzenszwalb

public:
    UnionFind(int elements);
    int find(int i);
    void unite(int i, int j);
    int getSize(int i);
};

#endif