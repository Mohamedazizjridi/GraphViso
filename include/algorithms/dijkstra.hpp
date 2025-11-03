#pragma once
#include <vector>
#include <unordered_map>
#include "../core/graph.hpp"
using namespace std;
namespace graphviso {
namespace algorithms {

struct PathInfo {
    vector<int> path;
    double distance;
};

class Dijkstra {
public:
    PathInfo findShortestPath(const Graph& graph, int startNodeId, int endNodeId);
    
private:
       unordered_map<int, double> distances_;
        unordered_map<int, int> previous_;
};

} // namespace algorithms
} // namespace graphviso
