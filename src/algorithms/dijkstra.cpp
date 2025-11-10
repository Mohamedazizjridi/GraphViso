#include "algorithms/dijkstra.hpp"
#include <bits/stdc++.h>
using namespace std;
namespace graphviso {
namespace algorithms {

PathInfo Dijkstra::findShortestPath(const Graph& graph, int startNodeId, int endNodeId) {
    distances_.clear();
    previous_.clear();
    for (const auto& node : graph.getNodes()) {
        distances_[node->getId()] = numeric_limits<double>::infinity();
    }
    distances_[startNodeId] = 0;
    auto comparator = [this](int a, int b) {
        return distances_[a] > distances_[b];
    };
    priority_queue<int, vector<int>, decltype(comparator)> queue(comparator);
    
    queue.push(startNodeId);
    
    while (!queue.empty()) {
        int current = queue.top();
        queue.pop();
        
        if (current == endNodeId) {
            break;
        }
        for (const auto& edge : graph.getEdges()) {
            if (edge->getSource()->getId() == current) {
                int neighbor = edge->getTarget()->getId();
                double newDist = distances_[current] + edge->getWeight();
                
                if (newDist < distances_[neighbor]) {
                    distances_[neighbor] = newDist;
                    previous_[neighbor] = current;
                    queue.push(neighbor);
                }
            }
            if (!graph.isDirected() && edge->getTarget()->getId() == current) {
                int neighbor = edge->getSource()->getId();
                double newDist = distances_[current] + edge->getWeight();
                
                if (newDist < distances_[neighbor]) {
                    distances_[neighbor] = newDist;
                    previous_[neighbor] = current;
                    queue.push(neighbor);
                }
            }
        }
    }
    
    PathInfo result;
    result.distance = distances_[endNodeId];
    
    if (result.distance == numeric_limits<double>::infinity()) {
        return result;  
    }
    int current = endNodeId;
    while (current != startNodeId) {
        result.path.push_back(current);
        current = previous_[current];
    }
    result.path.push_back(startNodeId);
    reverse(result.path.begin(), result.path.end());
    return result;
}

} // namespace algorithms
} // namespace graphviso
