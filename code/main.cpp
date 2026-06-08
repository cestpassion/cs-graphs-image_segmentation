#include "CLI.hpp"
#include "Image.hpp"

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
        Nesta Issue 1, ainda nao foram implementados os algoritmos de segmentacao.

        Por enquanto, o programa faz o metodo "copy":
        - carrega a imagem;
        - opcionalmente converte para cinza;
        - salva a imagem de saida.

        Os metodos felzenszwalb, cousty e ift serao implementados nas proximas Issues.
    */
    if (options.method != "copy") {
        std::cout << "Aviso: o metodo '" << options.method << "' ainda sera implementado em outra Issue." << std::endl;
        std::cout << "Nesta etapa, o programa apenas salvará uma copia/pre-processamento da imagem." << std::endl;
    }

    if (!image.save(options.outputPath)) {
        std::cerr << "Nao foi possivel salvar a imagem de saida." << std::endl;
        return 1;
    }

    std::cout << "Imagem salva com sucesso em: " << options.outputPath << std::endl;

    return 0;
}