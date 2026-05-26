# Trabalho de Teoria de Grafos e Computabilidade - *Segmentação de Imagem*

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
```

## Equipe

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/artuh21" title="Arthur">
        <img src="https://avatars.githubusercontent.com/u/130799136?v=4" width="100px;" alt="Foto do Arthur"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Arthur%20Campos-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Arthur Campos">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/cestpassion" title="Bruna">
        <img src="https://avatars.githubusercontent.com/u/164122042?v=4" width="100px;" alt="Foto da Bruna"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Bruna%20Furtado-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Bruna Furtado">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/GabrielDev0001" title="Gabriel">
        <img src="https://avatars.githubusercontent.com/u/157509318?v=4" width="100px;" alt="Foto do Gabriel"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Gabriel%20Henrique-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Gabriel Henrique">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/FelipeeBarros" title="Felipe">
        <img src="https://avatars.githubusercontent.com/u/181682076?v=4" width="100px;" alt="Foto do Felipe"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Felipe%20Barros-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Felipe Barros">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MiguelPessoaLF" title="Miguel">
        <img src="https://avatars.githubusercontent.com/u/180358485?v=4" width="100px;" alt="Foto do Miguel"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Miguel%20Pessoa-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Miguel Pessoa">
        </sub>
      </a>
    </td>
  </tr>
</table>
