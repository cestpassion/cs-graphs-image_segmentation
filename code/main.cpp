#include "CLI.hpp"
#include "Image.hpp"
#include "Graph.hpp"
#include "FelzenszwalbSegmenter.hpp"
#include <vector>

#include <iostream>

int main(int argc, char* argv[]) {
    CLIOptions options = CLI::parse(argc, argv);

    if (options.help) {
        CLI::printHelp();
        return 0;
    }

    if (!options.valid) {
        std::cerr << "Erro: " << options.errorMessage << std::endl;
        std::cerr << std::endl;
        CLI::printHelp();
        return 1;
    }

    if (options.inputPath.empty() || options.outputPath.empty()) {
        std::cerr << "Erro: --input e --output sao obrigatorios." << std::endl;
        std::cerr << std::endl;
        CLI::printHelp();
        return 1;
    }

    Image image;

    if (!image.load(options.inputPath)) {
        std::cerr << "Nao foi possivel carregar a imagem de entrada." << std::endl;
        return 1;
    }

    std::cout << "Imagem carregada com sucesso." << std::endl;
    std::cout << "Largura: " << image.width << std::endl;
    std::cout << "Altura: " << image.height << std::endl;
    std::cout << "Canais: " << image.channels << std::endl;
    std::cout << "Metodo selecionado: " << options.method << std::endl;
    std::cout << "Vizinhanca selecionada: " << options.neighborhood << std::endl;

    if (options.gray) {
        image = image.toGrayscale();
        std::cout << "Imagem convertida para tons de cinza." << std::endl;
    }

    /*
        Câmbio issue 3 falando, primeiro algoritmo de segmentação implementado

        O programa agora suporta as seguintes operações:
        - "copy": Apenas carrega a imagem, aplica pré-processamento e a salva.
        - "felzenszwalb": Constrói o grafo e segmenta a imagem usando componentes disjuntos.

        Os métodos cousty e ift continuam pendentes para as próximas Issues.
    */

    
    if (options.method == "copy") {
        std::cout << "Executando metodo de copia simples..." << std::endl;
        // Não fazemos nada com o grafo aqui, a imagem já será salva ao final do script.
    }
    else if (options.method == "felzenszwalb") {
        std::cout << "Executando metodo de Felzenszwalb..." << std::endl;

        // 1. Define o tipo de vizinhanca (4 ou 8) a partir dos argumentos da CLI
        Connectivity conn = (options.neighborhood == 8) ? Connectivity::EIGHT : Connectivity::FOUR;

        // 2. Cria a funcao que calcula o peso (diferenca) entre dois pixels
        auto weightFunc = [&image](int u, int v) -> double {
            auto [xu, yu] = idToCoord(u, image.width);
            auto [xv, yv] = idToCoord(v, image.width);

            if (image.channels == 1) { // Imagem em tons de cinza
                int i1 = image.getPixel(xu, yu, 0);
                int i2 = image.getPixel(xv, yv, 0);
                return grayWeight(i1, i2);
            } else { // Imagem colorida (RGB)
                int r1 = image.getPixel(xu, yu, 0);
                int g1 = image.getPixel(xu, yu, 1);
                int b1 = image.getPixel(xu, yu, 2);
                int r2 = image.getPixel(xv, yv, 0);
                int g2 = image.getPixel(xv, yv, 1);
                int b2 = image.getPixel(xv, yv, 2);
                return colorWeight(r1, g1, b1, r2, g2, b2);
            }
        };

        // 3. Constroi a malha do grafo
        std::cout << "Construindo o grafo..." << std::endl;
        Graph graph = buildGraph(image.width, image.height, conn, weightFunc);

        // 4. Executa a logica de segmentacao
        std::cout << "Segmentando (k=" << options.k << ", min_size=" << options.minSize << ")..." << std::endl;
        std::vector<int> labels = segmentFelzenszwalb(graph, options.k, options.minSize);

        // 5. Transforma o vetor de labels de volta em uma imagem visualmente colorida
        std::cout << "Gerando imagem de saida..." << std::endl;
        image = Image::createColoredLabelImage(labels, image.width, image.height);
    }
    else {
        std::cout << "Aviso: o metodo '" << options.method << "' ainda sera implementado em outra Issue." << std::endl;
        std::cout << "A imagem sera salva sem alteracoes adicionais de segmentacao." << std::endl;
    }

    if (!image.save(options.outputPath)) {
        std::cerr << "Nao foi possivel salvar a imagem de saida." << std::endl;
        return 1;
    }

    std::cout << "Imagem salva com sucesso em: " << options.outputPath << std::endl;

    return 0;
}