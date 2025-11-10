#include "algorithms/traversal.hpp"
#include <bits/stdc++.h>
using namesapce std;

namespace graphviso {
namespace algorithms {

void BFS::traverse(const Graph& graph, int startNodeId, VisitCallback callback) {
    unordered_set<int> visited;
    queue<shared_ptr<Node>> queue;
    
    auto nodes = graph.getNodes();
    auto start = find_if(nodes.begin(), nodes.end(),
        [startNodeId](const shared_ptr<Node>& node) {
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
    unordered_set<int> visited;
    stack<std::shared_ptr<Node>> stack;
    
    auto nodes = graph.getNodes();
    auto start = find_if(nodes.begin(), nodes.end(),
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
        vector<shared_ptr<Node>> neighbors;
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
