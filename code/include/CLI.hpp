#ifndef CLI_HPP
#define CLI_HPP

#include <vector>
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
    std::vector<int> thresholds; // para que possa passar mais de um (ex: 20,40,80)

    std::string seedsPath;

  
    std::vector<int> manualSeeds;

    bool autoSeeds = false;
    int autoSeedRows = 0;
    int autoSeedCols = 0;

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