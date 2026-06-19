#include "FelzenszwalbSegmenter.hpp"
#include "Graph.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <set>

// funcionando perfeitamente

// Teste 1: Imagem totalmente homogênea (pesos zero)
// Deve resultar em um único componente gigante, independentemente de k.
void testHomogeneousImage() {
    auto weightFunc = [](int, int) { return 0.0; };
    // Imagem 3x3
    Graph g = buildGraph(3, 3, Connectivity::FOUR, weightFunc);
    
    std::vector<int> labels = segmentFelzenszwalb(g, 100.0, 1);
    
    // Todos os pixels devem ter o mesmo label (pertencer ao mesmo componente)
    int firstLabel = labels[0];
    for (int label : labels) {
        assert(label == firstLabel);
    }
}

// Teste 2: Imagem com duas cores bem distintas (fronteira clara)
// Com k pequeno, deve separar em pelo menos 2 componentes.
void testClearDichotomy() {
    // Simula uma imagem 4x4 onde a metade esquerda (x < 2) é idêntica internamente
    // e a metade direita (x >= 2) também, mas a fronteira entre elas possui peso alto.
    auto weightFunc = [](int u, int v) {
        int x_u = u % 4;
        int x_v = v % 4;
        bool u_left = (x_u < 2);
        bool v_left = (x_v < 2);
        if (u_left != v_left) {
            return 255.0; // Fronteira de cor muito forte
        }
        return 0.0; // Mesma cor internamente
    };
    
    Graph g = buildGraph(4, 4, Connectivity::FOUR, weightFunc);
    
    // Com k baixo (50.0), o limite adaptativo não consegue superar o peso 255.0,
    // mantendo os dois lados separados.
    std::vector<int> labels = segmentFelzenszwalb(g, 50.0, 1);
    
    // O pixel id 0 (coluna 0) deve ser igual ao id 1 (coluna 1)
    assert(labels[0] == labels[1]);
    // O pixel id 2 (coluna 2) deve ser igual ao id 3 (coluna 3)
    assert(labels[2] == labels[3]);
    // Mas a esquerda deve ser totalmente diferente da direita
    assert(labels[0] != labels[2]);
}

// Teste 3: Filtro de tamanho mínimo (min_size)
// Um componente menor que o min_size deve ser forçado a se fundir com o vizinho.
void testMinSizeEnforcement() {
    // Grafo em linha de 3 pixels (1x3). O do meio é isolado por fronteiras pesadas.
    auto weightFunc = [](int u, int v) {
        if ((u == 0 && v == 1) || (u == 1 && v == 0)) return 100.0;
        if ((u == 1 && v == 2) || (u == 2 && v == 1)) return 100.0;
        return 0.0;
    };
    
    Graph g = buildGraph(3, 1, Connectivity::FOUR, weightFunc);
    
    // k = 10 (não fundiria naturalmente pelo peso ser 100), mas min_size = 2.
    // O pixel do meio (id 1) começa com tamanho 1, então viola o min_size e DEVE se fundir.
    std::vector<int> labels = segmentFelzenszwalb(g, 10.0, 2);
    
    std::set<int> uniqueLabels(labels.begin(), labels.end());
    // Em vez de 3 componentes isolados, devemos ter no máximo 2 devido à fusão forçada.
    assert(uniqueLabels.size() < 3);
}

int main() {
    testHomogeneousImage();
    testClearDichotomy();
    testMinSizeEnforcement();
    std::cout << "Todos os testes de Felzenszwalb passaram com sucesso!" << std::endl;
    return 0;
}


//          ╱|、
//         (˚ˎ 。7  
//          |、˜〵      
//          じしˍ,)ノ