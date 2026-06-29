# Trabalho de Teoria de Grafos e Computabilidade - *Segmentação de Imagem*
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![LaTeX](https://img.shields.io/badge/LaTeX-JBCS_Template-green.svg)

Projeto desenvolvido para a disciplina de **Teoria de Grafos e Computabilidade**, com o objetivo de estudar e implementar métodos de **segmentação de imagens baseada em grafos**.

A proposta consiste em modelar uma imagem digital como um grafo, em que cada pixel é representado por um vértice e as relações de vizinhança entre pixels são representadas por arestas ponderadas. A segmentação é então obtida por meio do particionamento dos vértices do grafo.

## Objetivo

Implementar e comparar diferentes abordagens de segmentação de imagens usando grafos, considerando imagens em níveis de cinza e imagens coloridas.

Os métodos estudados são:

1. Segmentação baseada em **Árvore Geradora Mínima**, conforme o método de [Felzenszwalb e Huttenlocher](refs/Efficient_Graph-Based_Image_Segmentation.pdf).
2. Segmentação hierárquica baseada em **Árvore Geradora Mínima**, conforme o método de [Cousty et al](refs/Hierarchical_Segmentations_with_Graphs_Quasi-flat_.pdf).
3. Segmentação baseada em **Caminho Mínimo**, utilizando a abordagem de [Image Foresting Transform, IFT](refs/The_Image_Foresting_Transform_Theory_Algorithms_an.pdf).

## Métodos previstos

### Felzenszwalb e Huttenlocher

Método baseado em grafos que utiliza uma estratégia relacionada à Árvore Geradora Mínima. A imagem é modelada como um grafo e os componentes são unidos de acordo com a diferença entre regiões e um limiar adaptativo.

### Cousty et al.

Método de segmentação hierárquica baseado em grafos, envolvendo conceitos como zonas quasi-flat, árvores geradoras mínimas e mapas de saliência.

### Image Foresting Transform, IFT

Método baseado em caminho mínimo. A segmentação ocorre a partir de sementes, e os pixels são conquistados por árvores de caminhos ótimos.

## Estrutura do repositório

```txt
.
├── code/
│   ├── data/
│   │   └── input/
│   │       ├── alien.png
│   │       ├── cenario.jpg
│   │       ├── dados.png
│   │       ├── flor.jpg
│   │       ├── seeds.txt
│   │       └── shrek.png
│   ├── external/
│   │   ├── README.md
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   ├── include/
│   │   ├── CLI.hpp
│   │   ├── CoustySegmenter.hpp
│   │   ├── Edge.hpp
│   │   ├── FelzenszwalbSegmenter.hpp
│   │   ├── Graph.hpp
│   │   ├── IFTSegmenter.hpp
│   │   ├── Image.hpp
│   │   ├── MST.hpp
│   │   ├── PriorityQueue.hpp
│   │   └── UnionFind.hpp
│   ├── src/
│   │   ├── CLI.cpp
│   │   ├── CoustySegmenter.cpp
│   │   ├── FelzenszwalbSegmenter.cpp
│   │   ├── Graph.cpp
│   │   ├── IFTSegmenter.cpp
│   │   ├── Image.cpp
│   │   ├── MST.cpp
│   │   ├── PriorityQueue.cpp
│   │   └── UnionFind.cpp
│   ├── tests/
│   │   ├── .gitkeep
│   │   ├── test_cousty.cpp
│   │   ├── test_felzenszwalb.cpp
│   │   ├── test_graph.cpp
│   │   └── test_ift.cpp
│   ├── Makefile
│   └── main.cpp
├── docs/
│   └── 2026-1-assignment-1-statement.pdf
├── refs/
│   ├── Efficient_Graph-Based_Image_Segmentation.pdf
│   ├── Hierarchical_Segmentations_with_Graphs_Quasi-flat_.pdf
│   └── The_Image_Foresting_Transform_Theory_Algorithms_an.pdf
├── report/
│   ├── figs/
│   │   ├── alien_cousty_t20_boundary.png
│   │   ├── alien_cousty_t80_boundary.png
│   │   ├── alien_felzenszwalb_boundary.png
│   │   ├── cenario_cousty_t40_boundary.png
│   │   ├── cenario_cousty_t80_avg.png
│   │   ├── cenario_felzenszwalb_boundary.png
│   │   ├── dados_cousty_saliency.png
│   │   ├── flor_cousty_t80_avg.png
│   │   ├── flor_ift_boundary.png
│   │   ├── shrek_cousty_t20_boundary.png
│   │   ├── shrek_felzenszwalb_boundary.png
│   │   └── shrek_ift_boundary.png
│   ├── aas_macros.sty
│   ├── academicons.sty
│   ├── academicons.ttf
│   ├── apalike-sol.bst
│   ├── main.tex
│   ├── refs.bib
│   ├── sbc2023.cls
│   ├── sectsty.sty
│   ├── tuacademicons.fd
│   └── Relatório do Projeto.pdf
├── results/
│   ├── alien/
│   │   ├── alien_cousty_saliency.png
│       ...
│   │   └── alien_ift_random.png
│   ├── cenario/
│   │   ├── cenario_cousty_saliency.png
│       ...
│   │   └── cenario_ift_random.png
│   ├── dados/
│   │   ├── dados_cousty_saliency.png
│       ...
│   │   └── dados_ift_random.png
│   ├── flor/
│   │   ├── flor_cousty_saliency.png
│       ...
│   │   └── flor_ift_random.png
│   └── shrek/
│       ├── shrek_cousty_saliency.png
│       ...
│       └── shrek_ift_random.png
├── .gitignore
├── LICENSE
└── README.md
````

## Pastas principais

| Pasta               | Descrição                                            |
| ------------------- | ---------------------------------------------------- |
| `code/`             | Código-fonte em C++                                  |
| `code/include/`     | Arquivos de cabeçalho `.hpp`                         |
| `code/src/`         | Arquivos de implementação `.cpp`                     |
| `code/external/`    | Bibliotecas externas header-only                     |
| `code/data/input/`  | Imagens de entrada                                   |
| `code/data/output/` | Imagens geradas pelo programa                        |
| `docs/`             | Documentação auxiliar do projeto                     |
| `refs/`             | Artigos e referências científicas usadas no trabalho |
| `report/`           | Relatório final em LaTeX e PDF                       |

## Dependências

O projeto utiliza as bibliotecas header-only:

* `stb_image.h`
* `stb_image_write.h`

Essas bibliotecas são usadas para leitura e escrita de imagens.

Elas devem estar na pasta:

```txt
code/external/
```

## Como compilar

Entre na pasta do código:

```bash
cd code
```

Compile com:

```bash
make
```

ou 

```bash
mingw32-make
```

Isso gera o executável:

```txt
segmentador
```

No Windows, o executável pode ser gerado como:

```txt
segmentador.exe
```

## Como limpar a compilação

Dentro da pasta `code/`, execute:

```bash
make clean
```

ou 

```bash
mingw32-make clean
```

Esse comando remove o executável gerado.

## Como executar

### Exibir ajuda

```bash
./segmentador --help
```

### Copiar imagem colorida

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/copia.png --method copy --color
```

### Converter imagem para tons de cinza

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/cinza.png --method copy --gray
```

## Resultados Experimentais

O diretório `results/` contém o conjunto completo de segmentações geradas pelos três métodos (Felzenszwalb, Cousty, IFT) para todas as imagens de teste. Foram geradas visualizações de borda (`_boundary`), médias (`_avg`), segmentação aleatória (`_random`) e mapas auxiliares (saliência e custo), totalizando 85 arquivos de análise qualitativa organizados por imagem de entrada.

## Parâmetros aceitos

| Parâmetro        | Descrição                             |
| ---------------- | ------------------------------------- |
| `--input`        | Caminho da imagem de entrada ou diretório (se usar `--batch`) |
| `--output`       | Caminho da imagem de saída ou diretório (se usar `--batch`) |
| `--method`       | Método selecionado (`copy`, `felzenszwalb`, `cousty`, `ift`) |
| `--gray`         | Converte a imagem para tons de cinza  |
| `--color`        | Mantém a imagem colorida              |
| `--neighborhood` | Define vizinhança 4 ou 8              |
| `--k`            | Parâmetro do método de Felzenszwalb   |
| `--min_size`     | Tamanho mínimo de componente (F&H)    |
| `--threshold`    | Limiar usado no método hierárquico (Cousty) |
| `--thresholds`   | Lista de limiares separados por vírgula (ex: `20,40,80`) |
| `--seeds`        | Arquivo de sementes para o método IFT |
| `--seed`         | Insere semente manualmente (formato: `x y label`). Pode ser repetido |
| `--auto-seeds`   | Gera grade automática de sementes (formato: `linhas colunas`) |
| `--median`       | Aplica filtro de mediana 3x3 antes da segmentação para reduzir ruídos |
| `--batch`        | Processa todas as imagens do diretório de entrada de uma só vez |
| `--help`         | Exibe a ajuda do programa             |

## Exemplos futuros de execução

### Felzenszwalb

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/felzenszwalb.png --method felzenszwalb --k 300 --min_size 20 --neighborhood 8
```

### Cousty

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/cousty.png --method cousty --threshold 40 --neighborhood 8
```

### IFT

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/ift.png --method ift --seeds data/input/seeds.txt --neighborhood 8
```

### Processamento em Lote (Batch Mode) e Filtro de Mediana
Processa todas as imagens da pasta de entrada, aplicando o filtro de mediana 3x3:
```bash
./segmentador --input data/input/ --output data/output/ --method felzenszwalb --k 300 --median --batch
```

## Saídas Geradas

Ao executar os algoritmos de segmentação, o programa gera automaticamente múltiplas visualizações para facilitar a análise comparativa:

* `_random.png`: Cada segmento recebe uma cor aleatória, destacando a partição.
* `_avg.png`: Cada segmento é preenchido com a cor média daquela região na imagem original.
* `_boundary.png`: Desenha as fronteiras dos segmentos em vermelho sobre a imagem original.

**Mapas Intermediários:**
* `_cousty_saliency.png`: Mapa de saliência gerado pelo método de Cousty.
* `_ift_cost.png`: Mapa de custos indicando a dificuldade de conquista de cada pixel pelas sementes no método IFT.

## Documentação

A documentação auxiliar está em:

```txt
docs/
```

## Referências

Os artigos científicos usados como base para o trabalho estão na pasta:

```txt
refs/
```

Referências principais:

* Pedro F. Felzenszwalb e Daniel P. Huttenlocher — Efficient Graph-Based Image Segmentation.
* Jean Cousty, Laurent Najman, Yukiko Kenmochi e Silvio Jamil Ferzoli Guimarães — Hierarchical Segmentations with Graphs.
* Alexandre X. Falcão, Jorge Stolfi e Roberto de Alencar Lotufo — The Image Foresting Transform.

## Relatório

O relatório final será desenvolvido na pasta:

```txt
report/
```

## Equipe

<table width="560">
  <tr>
    <td align="center" width="140">
      <a href="https://github.com/cestpassion" title="Bruna Furtado">
        <img src="https://avatars.githubusercontent.com/u/164122042?v=4" width="100px;" alt="Foto da Bruna"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Bruna%20Furtado-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Bruna Furtado">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/DomynicBl" title="Domynic Barros Lima">
        <img src="https://github.com/DomynicBl.png" width="100px;" alt="Foto do Domynic"/>
        <sub>
          <img src="https://img.shields.io/badge/-Domynic%20Barros%20Lima-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Domynic Barros Lima">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/flp2113" title="Felipe Guerzoni">
        <img src="https://avatars.githubusercontent.com/u/161882746?v=4" width="100px;" alt="Foto do Felipe"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Felipe%20Guerzoni-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Felipe Guerzoni">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/FelipeMizher" title="Felipe Mizher">
        <img src="https://avatars.githubusercontent.com/u/130677681?v=4" width="100px;" alt="Foto do Felipe"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Felipe%20Mizher-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Felipe Mizher">
        </sub>
      </a>
    </td>
  </tr>
</table>

<table width="560">
  <tr>
    <td align="center" width="140">
      <a href="https://github.com/GabrielDev0001" title="Gabriel Henrique">
        <img src="https://avatars.githubusercontent.com/u/157509318?v=4" width="100px;" alt="Foto do Gabriel"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Gabriel%20Henrique-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Gabriel Henrique">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/MarcosVettel" title="Marcos Paulo">
        <img src="https://avatars.githubusercontent.com/u/134240264?v=4" width="100px;" alt="Foto do Marcos"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Marcos%20Paulo-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Marcos Paulo">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/mioj0kt" title="Matheus Felipe">
        <img src="https://avatars.githubusercontent.com/u/161849185?v=4" width="100px;" alt="Foto do Matheus"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Matheus%20Felipe-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Matheus Felipe">
        </sub>
      </a>
    </td>
    <td align="center" width="140">
      <a href="https://github.com/paulogab2601" title="Paulo Gabriel">
        <img src="https://avatars.githubusercontent.com/u/138730369?v=4" width="100px;" alt="Foto do Paulo"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Paulo%20Gabriel-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Paulo Gabriel">
        </sub>
      </a>
    </td>
  </tr>
</table>
