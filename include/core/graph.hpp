#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "node.hpp"
#include "edge.hpp"

namespace graphviso {

class Graph {
public:
    Graph(bool isDirected = false);
    
    std::shared_ptr<Node> addNode(double x, double y);
    void removeNode(int id);
    void addEdge(int sourceId, int targetId, double weight = 1.0);
    void removeEdge(int sourceId, int targetId);
    
    bool isDirected() const;
    std::vector<std::shared_ptr<Node>> getNodes() const;
    std::vector<std::shared_ptr<Edge>> getEdges() const;
    
    void clear();
    void save(const std::string& filename) const;
    void load(const std::string& filename);

private:
    bool isDirected_;
    std::unordered_map<int, std::shared_ptr<Node>> nodes_;
    std::vector<std::shared_ptr<Edge>> edges_;
    int nextNodeId_;
};

} // namespace graphviso