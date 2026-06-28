#include "CLI.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

std::vector<int> parseThresholdList(const std::string& value, bool& valid, std::string& errorMessage) {
    // A lista foi implementada como CSV simples para manter a CLI legível sem
    // introduzir dependências novas ou sintaxe mais pesada.
    std::vector<int> thresholds;
    std::stringstream stream(value);
    std::string token;

    while (std::getline(stream, token, ',')) {
        if (token.empty()) {
            valid = false;
            errorMessage = "A lista de thresholds nao pode conter valores vazios.";
            return {};
        }

        try {
            thresholds.push_back(std::stoi(token));
        } catch (const std::exception&) {
            valid = false;
            errorMessage = "O valor de --thresholds deve conter inteiros separados por virgula.";
            return {};
        }
    }

    if (thresholds.empty()) {
        valid = false;
        errorMessage = "O argumento --thresholds exige pelo menos um valor.";
    }

    return thresholds;
}

}  // namespace

CLIOptions CLI::parse(int argc, char* argv[]) {
    CLIOptions options;

    auto requireValue = [&](int& i, const std::string& argName) {
        if (i + 1 >= argc) {
            options.valid = false;
            options.errorMessage = "Falta valor para o argumento " + argName;
        } else {
            i++;
        }
    };

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input" || arg == "-i") {
            requireValue(i, arg);
            if (options.valid) options.inputPath = argv[i];
        } else if (arg == "--output" || arg == "-o") {
            requireValue(i, arg);
            if (options.valid) options.outputPath = argv[i];
        } else if (arg == "--method" || arg == "-m") {
            requireValue(i, arg);
            if (options.valid) options.method = argv[i];
        } else if (arg == "--color") {
            options.color = true;
        } else if (arg == "--gray") {
            options.gray = true;
        } else if (arg == "--k") {
            requireValue(i, arg);
            if (options.valid) options.k = std::stod(argv[i]);
        } else if (arg == "--min_size") {
            requireValue(i, arg);
            if (options.valid) options.minSize = std::stoi(argv[i]);
        } else if (arg == "--threshold") {
            requireValue(i, arg);
            if (options.valid) options.threshold = std::stoi(argv[i]);
        } else if (arg == "--thresholds") {
            requireValue(i, arg);
            if (options.valid) options.thresholds = parseThresholdList(argv[i], options.valid, options.errorMessage);
        } else if (arg == "--seeds") {
            requireValue(i, arg);
            if (options.valid) options.seedsPath = argv[i];
        } else if (arg == "--seed") {
            requireValue(i, arg);
            if (options.valid) options.manualSeeds.push_back(std::stoi(argv[i]));
            requireValue(i, arg);
            if (options.valid) options.manualSeeds.push_back(std::stoi(argv[i]));
            requireValue(i, arg);
            if (options.valid) options.manualSeeds.push_back(std::stoi(argv[i]));
        } else if (arg == "--auto-seeds") {
            options.autoSeeds = true;
            requireValue(i, arg);
            if (options.valid) options.autoSeedRows = std::stoi(argv[i]);
            requireValue(i, arg);
            if (options.valid) options.autoSeedCols = std::stoi(argv[i]);
        } else if (arg == "--median") {
            options.useMedian = true;
        } else if (arg == "--batch") {
            options.batchMode = true;
        } else if (arg == "--help" || arg == "-h") {
            options.help = true;
        } else {
            options.valid = false;
            options.errorMessage = "Argumento desconhecido: " + arg;
        }

        if (!options.valid) break;
    }

    if (options.valid && !options.help) {
        if (options.inputPath.empty()) {
            options.valid = false;
            options.errorMessage = "O caminho de entrada (--input) e obrigatorio.";
        }
        if (!options.batchMode && options.outputPath.empty()) {
            options.valid = false;
            options.errorMessage = "O caminho de saida (--output) e obrigatorio.";
        }
    }

    return options;
}

void CLI::printHelp() {
    std::cout << "Uso: segmentador [opcoes]\n\n";
    std::cout << "Opcoes principais:\n";
    std::cout << "  --input <caminho>        Caminho da imagem de entrada ou diretorio (batch)\n";
    std::cout << "  --output <caminho>       Caminho da imagem de saida ou diretorio (batch)\n";
    std::cout << "  --method <metodo>        Metodo de segmentacao (copy, felzenszwalb, cousty, ift)\n";
    std::cout << "  --color                  Forca leitura em modo colorido (RGB)\n";
    std::cout << "  --gray                   Forca leitura em tons de cinza\n\n";

    std::cout << "Novas Funcionalidades (Batch & Filtro):\n";
    std::cout << "  --median                 Aplica filtro de mediana 3x3 antes do processamento\n";
    std::cout << "  --batch                  Processa todos os arquivos do diretorio indicado em --input\n\n";

    std::cout << "Parametros especificos:\n";
    std::cout << "  --k <valor>              Parametro k (Felzenszwalb)\n";
    std::cout << "  --min_size <valor>       Tamanho minimo de componente\n";
    std::cout << "  --threshold <valor>      Limiar usado no metodo hierarquico\n";
    std::cout << "  --thresholds <a,b,c>     Lista de limiares para varios niveis\n\n";

    std::cout << "Sementes do metodo IFT (escolha uma das formas):\n";
    std::cout << "  --seeds <arquivo>        Arquivo de sementes (linhas 'x y label')\n";
    std::cout << "  --seed <x> <y> <label>   Semente manual; pode repetir o argumento\n";
    std::cout << "  --auto-seeds <r> <c>     Grade automatica de r x c sementes\n\n";

    std::cout << "Exemplo Batch:\n";
    std::cout << "  ./segmentador --input data/input/ --output data/output/batch/ --method felzenszwalb --k 300 --median --batch\n";
}