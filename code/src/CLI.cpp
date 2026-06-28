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

    auto requireValue = [&](int& i, const std::string& argName) -> std::string {
        if (i + 1 >= argc) {
            options.valid = false;
            options.errorMessage = "Falta valor para o argumento " + argName;
            return "";
        }
        return argv[++i];
    };

    auto requireInt = [&](int& i, const std::string& argName, bool& ok) -> int {
        ok = false;
        std::string val = requireValue(i, argName);
        if (!options.valid) return 0;
        try {
            int parsed = std::stoi(val);
            ok = true;
            return parsed;
        } catch (...) {
            options.valid = false;
            options.errorMessage = "O argumento " + argName + " deve ser um numero inteiro.";
            return 0;
        }
    };

    auto requireDouble = [&](int& i, const std::string& argName, bool& ok) -> double {
        ok = false;
        std::string val = requireValue(i, argName);
        if (!options.valid) return 0.0;
        try {
            double parsed = std::stod(val);
            ok = true;
            return parsed;
        } catch (...) {
            options.valid = false;
            options.errorMessage = "O argumento " + argName + " deve ser numerico.";
            return 0.0;
        }
    };

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input" || arg == "-i") {
            options.inputPath = requireValue(i, arg);
        } else if (arg == "--output" || arg == "-o") {
            options.outputPath = requireValue(i, arg);
        } else if (arg == "--method" || arg == "-m") {
            options.method = requireValue(i, arg);
        } else if (arg == "--color") {
            options.color = true;
        } else if (arg == "--gray") {
            options.gray = true;
        } else if (arg == "--neighborhood") {
            bool ok;
            options.neighborhood = requireInt(i, arg, ok);
        } else if (arg == "--k") {
            bool ok;
            options.k = requireDouble(i, arg, ok);
        } else if (arg == "--min_size") {
            bool ok;
            options.minSize = requireInt(i, arg, ok);
        } else if (arg == "--threshold") {
            bool ok;
            options.threshold = requireInt(i, arg, ok);
        } else if (arg == "--thresholds") {
            std::string val = requireValue(i, arg);
            if (options.valid) options.thresholds = parseThresholdList(val, options.valid, options.errorMessage);
        } else if (arg == "--seeds") {
            options.seedsPath = requireValue(i, arg);
        } else if (arg == "--seed") {
            bool okX, okY, okL;
            int x = requireInt(i, "--seed (x)", okX);
            if (!options.valid) break;
            int y = requireInt(i, "--seed (y)", okY);
            if (!options.valid) break;
            int label = requireInt(i, "--seed (label)", okL);
            if (!options.valid) break;
            if (okX && okY && okL) {
                options.manualSeeds.push_back(x);
                options.manualSeeds.push_back(y);
                options.manualSeeds.push_back(label);
            }
        } else if (arg == "--auto-seeds") {
            options.autoSeeds = true;
            bool okR, okC;
            options.autoSeedRows = requireInt(i, "--auto-seeds (rows)", okR);
            if (!options.valid) break;
            options.autoSeedCols = requireInt(i, "--auto-seeds (cols)", okC);
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
        if (options.outputPath.empty()) {
            options.valid = false;
            options.errorMessage = "O caminho de saida (--output) e obrigatorio.";
        }
        if (options.gray && options.color) {
            options.valid = false;
            options.errorMessage = "Use apenas --gray ou --color, nao ambos.";
        }
        if (options.neighborhood != 4 && options.neighborhood != 8) {
            options.valid = false;
            options.errorMessage = "O valor de --neighborhood deve ser 4 ou 8.";
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
    std::cout << "  --gray                   Forca leitura em tons de cinza\n";
    std::cout << "  --neighborhood <4|8>     Tipo de vizinhanca dos pixels\n\n";

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