# Trabalho de Teoria de Grafos e Computabilidade - *Segmentação de Imagem*

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
│   ├── external/
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   ├── include/
│   │   ├── CLI.hpp
│   │   └── Image.hpp
│   ├── src/
│   │   ├── CLI.cpp
│   │   └── Image.cpp
│   ├── tests/
│   ├── data/
│   │   ├── input/
│   │   └── output/
│   ├── main.cpp
│   └── Makefile
│
├── docs/
│   ├── 2026-1-assignment-1-statement.pdf
│   ├── README.md
│   ├── comandos.md
│   ├── arquitetura.md
│   ├── decisoes_do_projeto.md
│   ├── padrao_branches.md
│   └── cronograma.md
│
├── refs/
│   ├── Efficient_Graph-Based_Image_Segmentation.pdf
│   ├── Hierarchical_Segmentations_with_Graphs.pdf
│   └── The_Image_Foresting_Transform.pdf
│
├── report/
│   ├── main.tex
│   ├── references.bib
│   ├── main.pdf
│   └── figures/
│
├── README.md
├── LICENSE
└── .gitignore
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

Linux/macOS/Git Bash:

```bash
./segmentador --help
```

Windows PowerShell/CMD:

```bash
.\segmentador.exe --help
```

### Copiar imagem colorida

Linux/macOS/Git Bash:

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/copia.png --method copy --color
```

Windows PowerShell/CMD:

```bash
.\segmentador.exe --input data/input/exemplo.jpg --output data/output/copia.png --method copy --color
```

### Converter imagem para tons de cinza

Linux/macOS/Git Bash:

```bash
./segmentador --input data/input/exemplo.jpg --output data/output/cinza.png --method copy --gray
```

Windows PowerShell/CMD:

```bash
.\segmentador.exe --input data/input/exemplo.jpg --output data/output/cinza.png --method copy --gray
```

## Parâmetros aceitos

| Parâmetro        | Descrição                             |
| ---------------- | ------------------------------------- |
| `--input`        | Caminho da imagem de entrada          |
| `--output`       | Caminho da imagem de saída            |
| `--method`       | Método selecionado                    |
| `--gray`         | Converte a imagem para tons de cinza  |
| `--color`        | Mantém a imagem colorida              |
| `--neighborhood` | Define vizinhança 4 ou 8              |
| `--k`            | Parâmetro do método de Felzenszwalb   |
| `--min_size`     | Tamanho mínimo de componente          |
| `--threshold`    | Limiar usado no método hierárquico    |
| `--seeds`        | Arquivo de sementes para o método IFT |
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

<table>
  <tr>
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
      <a href="https://github.com/flp2113" title="Felipe">
        <img src="https://avatars.githubusercontent.com/u/161882746?v=4" width="100px;" alt="Foto do Felipe"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Felipe%20Guerzoni-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Felipe Guerzoni">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/FelipeMizher" title="Felipe">
        <img src="https://avatars.githubusercontent.com/u/130677681?v=4" width="100px;" alt="Foto do Felipe"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Felipe%20Mizher-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Felipe Mizher">
        </sub>
      </a>
    </td>
</table>

<table>
    <td align="center">
      <a href="https://github.com/matheusmra" title="Matheus">
        <img src="https://avatars.githubusercontent.com/u/69125506?v=4" width="100px;" alt="Foto do Matheus"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Matheus%20de%20Almeida-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Matheus de Almeida">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/mioj0kt" title="Matheus">
        <img src="https://avatars.githubusercontent.com/u/161849185?v=4" width="100px;" alt="Foto do Matheus"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Matheus%20Felipe-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Matheus Felipe">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MarcosVettel" title="Matheus">
        <img src="https://avatars.githubusercontent.com/u/134240264?v=4" width="100px;" alt="Foto do Matheus"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Marcos%20Paulo-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Marcos Paulo">
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/paulogab2601" title="Matheus">
        <img src="https://avatars.githubusercontent.com/u/138730369?v=4" width="100px;" alt="Foto do Paulo"/><br>
        <sub>
          <img src="https://img.shields.io/badge/-Paulo%20Gabriel-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub Paulo Gabriel">
        </sub>
      </a>
    </td>
  </tr>
</table>
