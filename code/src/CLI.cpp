#include "CLI.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

CLIOptions CLI::parse(int argc, char* argv[]) {
    CLIOptions options;

    auto requireValue = [&](int& i, const std::string& argumentName) -> std::string {
        if (i + 1 >= argc) {
            options.valid = false;
            options.errorMessage = "O argumento " + argumentName + " exige um valor.";
            return "";
        }

        i++;
        return argv[i];
    };

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            options.help = true;
            return options;
        } else if (arg == "--input") {
            options.inputPath = requireValue(i, "--input");
        } else if (arg == "--output") {
            options.outputPath = requireValue(i, "--output");
        } else if (arg == "--method") {
            options.method = requireValue(i, "--method");
        } else if (arg == "--neighborhood") {
            std::string value = requireValue(i, "--neighborhood");

            try {
                options.neighborhood = std::stoi(value);
            } catch (const std::exception&) {
                options.valid = false;
                options.errorMessage = "O valor de --neighborhood deve ser 4 ou 8.";
                return options;
            }
        } else if (arg == "--gray") {
            options.gray = true;
        } else if (arg == "--color") {
            options.color = true;
        } else if (arg == "--k") {
            std::string value = requireValue(i, "--k");

            try {
                options.k = std::stod(value);
            } catch (const std::exception&) {
                options.valid = false;
                options.errorMessage = "O valor de --k deve ser numerico.";
                return options;
            }
        } else if (arg == "--min_size" || arg == "--min-size") {
            std::string value = requireValue(i, arg);

            try {
                options.minSize = std::stoi(value);
            } catch (const std::exception&) {
                options.valid = false;
                options.errorMessage = "O valor de " + arg + " deve ser inteiro.";
                return options;
            }
        } else if (arg == "--threshold") {
            std::string value = requireValue(i, "--threshold");

            try {
                options.threshold = std::stoi(value);
            } catch (const std::exception&) {
                options.valid = false;
                options.errorMessage = "O valor de --threshold deve ser inteiro.";
                return options;
            }
        } else if (arg == "--seeds") {
            options.seedsPath = requireValue(i, "--seeds");
        } else {
            options.valid = false;
            options.errorMessage = "Argumento desconhecido: " + arg;
            return options;
        }

        if (!options.valid) {
            return options;
        }
    }

    if (options.gray && options.color) {
        options.valid = false;
        options.errorMessage = "Use apenas --gray ou --color, nao ambos.";
        return options;
    }

    if (options.neighborhood != 4 && options.neighborhood != 8) {
        options.valid = false;
        options.errorMessage = "O valor de --neighborhood deve ser 4 ou 8.";
        return options;
    }

    return options;
}

void CLI::printHelp() {
    std::cout << "Uso:\n";
    std::cout << "  ./segmentador --input <imagem> --output <saida> [opcoes]\n\n";

    std::cout << "Opcoes obrigatorias:\n";
    std::cout << "  --input <caminho>        Caminho da imagem de entrada\n";
    std::cout << "  --output <caminho>       Caminho da imagem de saida\n\n";

    std::cout << "Opcoes gerais:\n";
    std::cout << "  --method <metodo>        Metodo: copy, felzenszwalb, cousty ou ift\n";
    std::cout << "  --gray                   Converte a imagem para tons de cinza\n";
    std::cout << "  --color                  Mantem a imagem colorida\n";
    std::cout << "  --neighborhood <4|8>     Tipo de vizinhanca dos pixels\n";
    std::cout << "  --help, -h               Mostra esta ajuda\n\n";

    std::cout << "Parametros futuros dos algoritmos:\n";
    std::cout << "  --k <valor>              Parametro do metodo de Felzenszwalb\n";
    std::cout << "  --min_size <valor>       Tamanho minimo de componente\n";
    std::cout << "  --threshold <valor>      Limiar usado no metodo hierarquico\n";
    std::cout << "  --seeds <arquivo>        Arquivo de sementes para IFT\n\n";

    std::cout << "Exemplos:\n";
    std::cout << "  ./segmentador --input data/input/flor.png --output data/output/copia.png --method copy --color\n";
    std::cout << "  ./segmentador --input data/input/flor.png --output data/output/cinza.png --method copy --gray\n";
    std::cout << "Exemplos apos a implementacao dos algoritmos:\n";
    std::cout << "  ./segmentador --input data/input/flor.png --output data/output/fh.png --method felzenszwalb --k 300 --min_size 20\n";
    std::cout << "  ./segmentador --input data/input/flor.png --output data/output/cousty.png --method cousty --threshold 40\n";
    std::cout << "  ./segmentador --input data/input/flor.png --output data/output/ift.png --method ift --seeds data/input/seeds.txt\n";
}