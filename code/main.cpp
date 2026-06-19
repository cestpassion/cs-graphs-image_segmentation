#include "CLI.hpp"
#include "CoustySegmenter.hpp"
#include "FelzenszwalbSegmenter.hpp"
#include "Image.hpp"
#include "Graph.hpp"
#include <vector>

#include <iostream>

namespace {

std::function<double(int, int)> makeWeightFunction(Image& image) {
    // Acabei criando esse método para evitar repetição de código

    return [&image](int u, int v) -> double {
        auto [xu, yu] = idToCoord(u, image.width);
        auto [xv, yv] = idToCoord(v, image.width);

        if (image.channels == 1) {
            return grayWeight(image.getPixel(xu, yu, 0), image.getPixel(xv, yv, 0));
        }

        return colorWeight(
            image.getPixel(xu, yu, 0), image.getPixel(xu, yu, 1), image.getPixel(xu, yu, 2),
            image.getPixel(xv, yv, 0), image.getPixel(xv, yv, 1), image.getPixel(xv, yv, 2)
        );
    };
}

bool endsWithSlash(const std::string& value) {
    return !value.empty() && (value.back() == '/' || value.back() == '\\');
}

std::string buildOutputName(const std::string& baseOutput, int threshold, bool multiLevel) {
    // Gerar por exemplo, cousty20.png, cousty40.png, etc.
    if (!multiLevel) {
        return baseOutput;
    }

    std::size_t slash = baseOutput.find_last_of("/\\");
    std::size_t dot = baseOutput.find_last_of('.');

    if (endsWithSlash(baseOutput) || dot == std::string::npos || (slash != std::string::npos && dot < slash)) {
        std::string directory = baseOutput;
        if (!endsWithSlash(directory)) {
            directory += '/';
        }
        return directory + "cousty" + std::to_string(threshold) + ".png";
    }

    return baseOutput.substr(0, dot) + std::to_string(threshold) + baseOutput.substr(dot);
}

std::string buildSaliencyName(const std::string& baseOutput, int threshold, bool multiLevel) {
    // O arquivo de saliência usa o mesmo padrão do segmento para facilitar a
    // leitura lado a lado durante a comparação das hierarquias.
    if (!multiLevel) {
        std::size_t slash = baseOutput.find_last_of("/\\");
        std::size_t dot = baseOutput.find_last_of('.');

        if (dot == std::string::npos || (slash != std::string::npos && dot < slash)) {
            return baseOutput + "_saliency.png";
        }

        return baseOutput.substr(0, dot) + "_saliency" + baseOutput.substr(dot);
    }

    std::size_t slash = baseOutput.find_last_of("/\\");
    std::string directory = (slash == std::string::npos) ? std::string() : baseOutput.substr(0, slash + 1);
    if (!endsWithSlash(baseOutput) && (slash == std::string::npos || baseOutput.find_last_of('.') != std::string::npos)) {
        directory = baseOutput.substr(0, slash + 1);
    }
    if (directory.empty()) {
        directory = "./";
    }
    return directory + "cousty" + std::to_string(threshold) + "_saliency.png";
}

}  // namespace

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
        auto weightFunc = makeWeightFunction(image);

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
    else if (options.method == "cousty") {
        std::cout << "Executando metodo de Cousty..." << std::endl;

        Connectivity conn = (options.neighborhood == 8) ? Connectivity::EIGHT : Connectivity::FOUR;
        auto weightFunc = makeWeightFunction(image);

        // Primeiro criamos o grafo da imagem e depois comprimimos isso em uma
        // MST. A hierarquia passa a depender só do limiar aplicado sobre essa árvore.
        std::cout << "Construindo o grafo..." << std::endl;
        Graph graph = buildGraph(image.width, image.height, conn, weightFunc);
        std::cout << "Construindo a MST..." << std::endl;
        MST mst = buildMST(graph);

        std::vector<int> thresholds = options.thresholds;
        if (thresholds.empty()) {
            thresholds.push_back(options.threshold);
        }

        bool multiLevel = thresholds.size() > 1;

        for (int threshold : thresholds) {
            // Cada threshold gera uma segmentação independente para permitir
            // comparação visual direta entre níveis mais finos e mais grossos.
            std::string thresholdMessage = "Segmentando (threshold=";
            thresholdMessage += std::to_string(threshold);
            thresholdMessage += ")...";
            std::cout << thresholdMessage << std::endl;

            std::vector<int> labels = segmentCoustyByThreshold(
                mst,
                static_cast<double>(threshold)
            );
            Image segmented = Image::createColoredLabelImage(
                labels,
                image.width,
                image.height
            );

            std::string outputName = buildOutputName(
                options.outputPath,
                threshold,
                multiLevel
            );
            if (!segmented.save(outputName)) {
                std::cerr << "Nao foi possivel salvar a imagem de saida." << std::endl;
                return 1;
            }

            std::string saliencyName = buildSaliencyName(
                options.outputPath,
                threshold,
                multiLevel
            );
            Image saliency = createCoustySaliencyImage(mst);
            saliency.save(saliencyName);

            if (!multiLevel) {
                image = segmented;
            }
        }

        if (multiLevel) {
            std::cout << "Multiplos niveis salvos com sucesso." << std::endl;
            return 0;
        }
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