#include "UnionFind.hpp"

UnionFind::UnionFind(int elements) {
    parent.resize(elements);
    rank.resize(elements, 0);
    size.resize(elements, 1); // Inicialmente, cada componente tem tamanho 1
    
    for (int i = 0; i < elements; ++i) {
        parent[i] = i; // Cada elemento é pai de si mesmo no início
    }
}

int UnionFind::find(int i) {
    if (parent[i] == i) {
        return i;
    }
    // Compressão de caminho (Path compression) para otimização
    return parent[i] = find(parent[i]);
}

void UnionFind::unite(int i, int j) {
    int rootI = find(i);
    int rootJ = find(j);

    if (rootI != rootJ) {
        // União baseada no rank para manter a árvore balanceada
        if (rank[rootI] < rank[rootJ]) {
            parent[rootI] = rootJ;
            size[rootJ] += size[rootI];
        } else if (rank[rootI] > rank[rootJ]) {
            parent[rootJ] = rootI;
            size[rootI] += size[rootJ];
        } else {
            parent[rootJ] = rootI;
            size[rootI] += size[rootJ];
            rank[rootI]++;
        }
    }
}

int UnionFind::getSize(int i) {
    return size[find(i)];
}