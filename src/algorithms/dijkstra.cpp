#include "algorithms/dijkstra.hpp"
#include <queue>
#include <limits>
#include <algorithm>

namespace graphviso {
namespace algorithms {

PathInfo Dijkstra::findShortestPath(const Graph& graph, int startNodeId, int endNodeId) {
    distances_.clear();
    previous_.clear();
    
    // Initialize distances
    for (const auto& node : graph.getNodes()) {
        distances_[node->getId()] = std::numeric_limits<double>::infinity();
    }
    distances_[startNodeId] = 0;
    
    // Priority queue with custom comparator for node IDs based on their distances
    auto comparator = [this](int a, int b) {
        return distances_[a] > distances_[b];
    };
    std::priority_queue<int, std::vector<int>, decltype(comparator)> queue(comparator);
    
    queue.push(startNodeId);
    
    while (!queue.empty()) {
        int current = queue.top();
        queue.pop();
        
        if (current == endNodeId) {
            break;
        }
        
        // Find all neighbors
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
    
    // Reconstruct path
    PathInfo result;
    result.distance = distances_[endNodeId];
    
    if (result.distance == std::numeric_limits<double>::infinity()) {
        return result;  // No path found
    }
    
    // Build path from end to start
    int current = endNodeId;
    while (current != startNodeId) {
        result.path.push_back(current);
        current = previous_[current];
    }
    result.path.push_back(startNodeId);
    
    // Reverse path to get start to end
    std::reverse(result.path.begin(), result.path.end());
    
    return result;
}

} // namespace algorithms
} // namespace graphviso