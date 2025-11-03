#pragma once

#include <vector>
#include <unordered_map>
#include "../core/graph.hpp"

namespace graphviso {
namespace algorithms {

struct PathInfo {
    std::vector<int> path;
    double distance;
};

class Dijkstra {
public:
    PathInfo findShortestPath(const Graph& graph, int startNodeId, int endNodeId);
    
private:
    std::unordered_map<int, double> distances_;
    std::unordered_map<int, int> previous_;
};

} // namespace algorithms
} // namespace graphviso