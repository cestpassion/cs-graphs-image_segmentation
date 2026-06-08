#ifndef CLI_HPP
#define CLI_HPP

#include <string>

struct CLIOptions {
    std::string inputPath;
    std::string outputPath;
    std::string method = "copy";

    int neighborhood = 4;

    bool gray = false;
    bool color = false;

    double k = 300.0;
    int minSize = 20;
    int threshold = 40;

    std::string seedsPath;

    bool help = false;
    bool valid = true;
    std::string errorMessage;
};

class CLI {
public:
    static CLIOptions parse(int argc, char* argv[]);
    static void printHelp();
};

#endif