#include "IFTSegmenter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "PriorityQueue.hpp"

namespace {
//issue #5
// Constroi uma lista de adjacencia (bidirecional) a partir das arestas do
// grafo. O Graph guarda cada aresta {u,v} uma unica vez (sem duplicata), mas a
// IFT precisa propagar nos dois sentidos, entao espelhamos cada aresta.
struct Neighbor {
    int vertex;
    double weight;
};

std::vector<std::vector<Neighbor>> buildAdjacency(const Graph& graph) {
    std::vector<std::vector<Neighbor>> adjacency(graph.numVertices);
    for (const Edge& edge : graph.edges) {
        adjacency[edge.src].push_back({edge.dest, edge.weight});
        adjacency[edge.dest].push_back({edge.src, edge.weight});
    }
    return adjacency;
}

}  // namespace

IFTResult runIFT(const Graph& graph, const std::vector<Seed>& seeds) {
    const int n = graph.numVertices;
    const double INF = std::numeric_limits<double>::infinity();

    IFTResult result;
    result.cost.assign(n, INF);
    result.predecessor.assign(n, -1);
    result.label.assign(n, 0);

    if (n <= 0) {
        return result;
    }

    std::vector<std::vector<Neighbor>> adjacency = buildAdjacency(graph);
    std::vector<char> processed(n, 0);  // pixel ja "fechado" (custo definitivo)

    PriorityQueue queue;

    // Inicializacao: cada semente vira a raiz de uma arvore, com custo 0 e o
    // seu proprio label. Esta e a diferenca central frente aos metodos por MST:
    // a propagacao parte das sementes, nao da estrutura global da imagem.
    for (const Seed& seed : seeds) {
        if (seed.x < 0 || seed.x >= graph.width || seed.y < 0 ||
            seed.y >= graph.height) {
            continue;  // fora dos limites; ignorada com seguranca
        }

        int id = coordToId(seed.x, seed.y, graph.width);

        // Se duas sementes caem no mesmo pixel, a de menor label (primeira a
        // chegar com custo 0) prevalece de forma deterministica.
        if (result.cost[id] == 0.0) {
            continue;
        }

        result.cost[id] = 0.0;
        result.predecessor[id] = -1;
        result.label[id] = seed.label;
        queue.push(0.0, id);
    }

    // Propagacao tipo Dijkstra com custo fmax.
    while (!queue.empty()) {
        PriorityQueue::Item item = queue.pop(result.cost);
        int p = item.vertex;

        if (p < 0 || processed[p]) {
            continue;
        }
        processed[p] = 1;

        double costP = result.cost[p];

        for (const Neighbor& neighbor : adjacency[p]) {
            int q = neighbor.vertex;
            if (processed[q]) {
                continue;
            }

            // Custo do caminho que passa por p e chega em q:
            //   cost(q) = max(cost(p), peso(p, q))
            double candidate = std::max(costP, neighbor.weight);

            if (candidate < result.cost[q]) {
                result.cost[q] = candidate;
                result.predecessor[q] = p;
                result.label[q] = result.label[p];  // herda a semente de p
                queue.push(candidate, q);
            }
        }
    }

    return result;
}

std::vector<int> segmentIFT(const Graph& graph, const std::vector<Seed>& seeds) {
    return runIFT(graph, seeds).label;
}

std::vector<Seed> loadSeedsFromFile(const std::string& path, int width,
                                    int height, bool& ok) {
    std::vector<Seed> seeds;
    std::ifstream file(path);

    if (!file.is_open()) {
        ok = false;
        return seeds;
    }
    ok = true;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        // Ignora espacos a esquerda para detectar comentarios/linhas vazias.
        std::size_t firstNonSpace = line.find_first_not_of(" \t\r\n");
        if (firstNonSpace == std::string::npos) {
            continue;  // linha em branco
        }
        if (line[firstNonSpace] == '#') {
            continue;  // comentario
        }

        std::istringstream stream(line);
        int x, y, label;
        if (!(stream >> x >> y >> label)) {
            std::cerr << "Aviso: linha " << lineNumber
                      << " do arquivo de sementes esta malformada e foi ignorada."
                      << std::endl;
            continue;
        }

        if (x < 0 || x >= width || y < 0 || y >= height) {
            std::cerr << "Aviso: semente (" << x << ", " << y
                      << ") fora dos limites da imagem; ignorada." << std::endl;
            continue;
        }

        seeds.push_back({x, y, label});
    }

    return seeds;
}

std::vector<Seed> generateAutomaticSeeds(int width, int height, int rows,
                                         int cols) {
    std::vector<Seed> seeds;

    if (width <= 0 || height <= 0 || rows <= 0 || cols <= 0) {
        return seeds;
    }

    // Distribui os pontos no centro de cada celula de uma grade rows x cols,
    // evitando as bordas extremas da imagem.
    int label = 1;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int x = static_cast<int>((c + 0.5) * width / cols);
            int y = static_cast<int>((r + 0.5) * height / rows);

            x = std::min(std::max(x, 0), width - 1);
            y = std::min(std::max(y, 0), height - 1);

            seeds.push_back({x, y, label});
            ++label;
        }
    }

    return seeds;
}

std::vector<Seed> seedsFromCoordinateList(const std::vector<int>& values,
                                          int width, int height, bool& ok) {
    std::vector<Seed> seeds;

    if (values.size() % 3 != 0) {
        ok = false;
        return seeds;
    }
    ok = true;

    for (std::size_t i = 0; i + 2 < values.size(); i += 3) {
        int x = values[i];
        int y = values[i + 1];
        int label = values[i + 2];

        if (x < 0 || x >= width || y < 0 || y >= height) {
            std::cerr << "Aviso: semente (" << x << ", " << y
                      << ") fora dos limites da imagem; ignorada." << std::endl;
            continue;
        }

        seeds.push_back({x, y, label});
    }

    return seeds;
}

Image createIFTCostImage(const IFTResult& result, int width, int height) {
    Image costImg;
    costImg.width = width;
    costImg.height = height;
    costImg.channels = 1;
    costImg.pixels.resize(width * height);

    double maxCost = 0.0;
    for (double c : result.cost) {
        if (c != std::numeric_limits<double>::infinity() && c > maxCost) {
            maxCost = c;
        }
    }

    for (std::size_t i = 0; i < result.cost.size(); ++i) {
        if (result.cost[i] == std::numeric_limits<double>::infinity()) {
            costImg.pixels[i] = 255;
        } else {
            double norm = maxCost > 0.0 ? (result.cost[i] / maxCost) : 0.0;
            costImg.pixels[i] = static_cast<unsigned char>(std::min(255.0, std::max(0.0, norm * 255.0)));
        }
    }
    return costImg;
}