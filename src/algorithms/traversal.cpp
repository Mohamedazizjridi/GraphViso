#include "algorithms/traversal.hpp"
#include <queue>
#include <unordered_set>
#include <stack>

namespace graphviso {
namespace algorithms {

void BFS::traverse(const Graph& graph, int startNodeId, VisitCallback callback) {
    std::unordered_set<int> visited;
    std::queue<std::shared_ptr<Node>> queue;
    
    auto nodes = graph.getNodes();
    auto start = std::find_if(nodes.begin(), nodes.end(),
        [startNodeId](const std::shared_ptr<Node>& node) {
            return node->getId() == startNodeId;
        });
        
    if (start == nodes.end()) {
        return;
    }
    
    queue.push(*start);
    visited.insert(startNodeId);
    
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        
        callback(*current);
        
        // Find all neighbors
        for (const auto& edge : graph.getEdges()) {
            if (edge->getSource()->getId() == current->getId()) {
                auto neighbor = edge->getTarget();
                if (visited.find(neighbor->getId()) == visited.end()) {
                    visited.insert(neighbor->getId());
                    queue.push(neighbor);
                }
            }
            if (!graph.isDirected() && edge->getTarget()->getId() == current->getId()) {
                auto neighbor = edge->getSource();
                if (visited.find(neighbor->getId()) == visited.end()) {
                    visited.insert(neighbor->getId());
                    queue.push(neighbor);
                }
            }
        }
    }
}

void DFS::traverse(const Graph& graph, int startNodeId, VisitCallback callback) {
    std::unordered_set<int> visited;
    std::stack<std::shared_ptr<Node>> stack;
    
    auto nodes = graph.getNodes();
    auto start = std::find_if(nodes.begin(), nodes.end(),
        [startNodeId](const std::shared_ptr<Node>& node) {
            return node->getId() == startNodeId;
        });
        
    if (start == nodes.end()) {
        return;
    }
    
    stack.push(*start);
    
    while (!stack.empty()) {
        auto current = stack.top();
        stack.pop();
        
        if (visited.find(current->getId()) != visited.end()) {
            continue;
        }
        
        visited.insert(current->getId());
        callback(*current);
        
        // Find all neighbors (push in reverse order to maintain expected traversal order)
        std::vector<std::shared_ptr<Node>> neighbors;
        for (const auto& edge : graph.getEdges()) {
            if (edge->getSource()->getId() == current->getId()) {
                auto neighbor = edge->getTarget();
                if (visited.find(neighbor->getId()) == visited.end()) {
                    neighbors.push_back(neighbor);
                }
            }
            if (!graph.isDirected() && edge->getTarget()->getId() == current->getId()) {
                auto neighbor = edge->getSource();
                if (visited.find(neighbor->getId()) == visited.end()) {
                    neighbors.push_back(neighbor);
                }
            }
        }
        
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            stack.push(*it);
        }
    }
}

} // namespace algorithms
} // namespace graphviso