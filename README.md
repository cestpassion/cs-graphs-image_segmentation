# sc-graphs-img-seg

Projeto desenvolvido para a disciplina de **Teoria de Grafos e Computabilidade**, com o objetivo de estudar e implementar métodos de **segmentação de imagens baseada em grafos**.

A proposta consiste em modelar uma imagem digital como um grafo, em que cada pixel é representado por um vértice e as relações de vizinhança entre pixels são representadas por arestas ponderadas. A segmentação é então obtida por meio do particionamento dos vértices do grafo.

## Objetivo

Implementar e comparar diferentes abordagens de segmentação de imagens usando grafos, considerando imagens em níveis de cinza e imagens coloridas.

Os métodos estudados são:

1. Segmentação baseada em **Árvore Geradora Mínima**, conforme o método de [Felzenszwalb e Huttenlocher](refs/Efficient_Graph-Based_Image_Segmentation.pdf).
2. Segmentação hierárquica baseada em **Árvore Geradora Mínima**, conforme o método de [Cousty et al](refs/Hierarchical_Segmentations_with_Graphs_Quasi-flat_.pdf).
3. Segmentação baseada em **Caminho Mínimo**, utilizando a abordagem de [Image Foresting Transform, IFT](refs/Efficient_Graph-Based_Image_Segmentation.pdf).

## Modelagem do problema

A imagem é transformada em um grafo ponderado não direcionado.

- Cada pixel da imagem corresponde a um vértice.
- Cada aresta conecta pixels vizinhos.
- O peso da aresta representa a diferença entre os pixels.
- O particionamento do grafo gera as regiões segmentadas da imagem.

Para imagens em níveis de cinza, o peso entre dois pixels pode ser calculado pela diferença absoluta entre suas intensidades:

```text
w(p, q) = |I(p) - I(q)|