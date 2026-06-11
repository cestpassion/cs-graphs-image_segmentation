#include "Graph.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <set>
#include <utility>
#include <vector>

static int edgeCount(int w, int h, Connectivity c)
{
    auto weight = [](int u, int v) { return static_cast<double>(u + v); };
    Graph g = buildGraph(w, h, c, weight);
    assert(g.numVertices == w * h);
    assert(g.width == w && g.height == h);
    std::set<std::pair<int, int>> seen;
    for (const Edge& e : g.edges)
    {
        assert(e.src != e.dest);
        int a = e.src < e.dest ? e.src : e.dest;
        int b = e.src < e.dest ? e.dest : e.src;
        assert(seen.insert({a, b}).second);
    }
    return static_cast<int>(g.edges.size());
}

static void testVertices()
{
    auto weight = [](int, int) { return 0.0; };
    Graph g = buildGraph(4, 5, Connectivity::FOUR, weight);
    assert(g.numVertices == 20);
    assert(g.width == 4 && g.height == 5);
}

static void testCoordId()
{
    for (int w = 1; w <= 8; ++w)
        for (int h = 1; h <= 8; ++h)
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                {
                    int id = coordToId(x, y, w);
                    assert(id == y * w + x);
                    auto p = idToCoord(id, w);
                    assert(p.first == x && p.second == y);
                }
}

static void testEdgesFour()
{
    assert(edgeCount(3, 3, Connectivity::FOUR) == 12);
    for (int w = 1; w <= 6; ++w)
        for (int h = 1; h <= 6; ++h)
            assert(edgeCount(w, h, Connectivity::FOUR) == w * (h - 1) + h * (w - 1));
}

static void testEdgesEight()
{
    assert(edgeCount(3, 3, Connectivity::EIGHT) == 20);
    for (int w = 1; w <= 6; ++w)
        for (int h = 1; h <= 6; ++h)
        {
            int e4 = w * (h - 1) + h * (w - 1);
            assert(edgeCount(w, h, Connectivity::EIGHT) == e4 + 2 * (w - 1) * (h - 1));
        }
}

static void testWeights()
{
    assert(grayWeight(10, 30) == 20.0);
    assert(grayWeight(30, 10) == 20.0);
    assert(grayWeight(7, 7) == 0.0);
    assert(colorWeight(0, 0, 0, 0, 3, 4) == 5.0);
    assert(colorWeight(5, 5, 5, 5, 5, 5) == 0.0);
    assert(std::fabs(colorWeight(0, 0, 0, 1, 2, 2) - 3.0) < 1e-9);
}

static void testWeightFromImage()
{
    std::vector<int> gray = {10, 12, 20, 40};
    auto weight = [&](int u, int v) { return grayWeight(gray[u], gray[v]); };
    Graph g = buildGraph(2, 2, Connectivity::FOUR, weight);
    for (const Edge& e : g.edges)
        assert(e.weight == grayWeight(gray[e.src], gray[e.dest]));
}

static void testNoDuplicates()
{
    auto weight = [](int u, int v) { return static_cast<double>(u + v); };
    Graph g = buildGraph(5, 4, Connectivity::EIGHT, weight);
    std::set<std::pair<int, int>> seen;
    for (const Edge& e : g.edges)
    {
        int a = e.src < e.dest ? e.src : e.dest;
        int b = e.src < e.dest ? e.dest : e.src;
        assert(seen.insert({a, b}).second);
    }
}

int main()
{
    testVertices();
    testCoordId();
    testEdgesFour();
    testEdgesEight();
    testWeights();
    testWeightFromImage();
    testNoDuplicates();
    std::printf("Todos os testes passaram\n");
    return 0;
}
