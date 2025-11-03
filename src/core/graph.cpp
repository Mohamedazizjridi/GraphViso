#include "core/graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace graphviso {

Graph::Graph(bool isDirected)
    : isDirected_(isDirected), nextNodeId_(0) {}

std::shared_ptr<Node> Graph::addNode(double x, double y) {
    auto node = std::make_shared<Node>(nextNodeId_++, x, y);
    nodes_[node->getId()] = node;
    return node;
}

void Graph::removeNode(int id) {
    if (nodes_.find(id) == nodes_.end()) {
        return;
    }
    
    // Remove all edges connected to this node
    edges_.erase(
        std::remove_if(edges_.begin(), edges_.end(),
            [id](const std::shared_ptr<Edge>& edge) {
                return edge->getSource()->getId() == id || 
                       edge->getTarget()->getId() == id;
            }
        ),
        edges_.end()
    );
    
    nodes_.erase(id);
}

void Graph::addEdge(int sourceId, int targetId, double weight) {
    auto sourceIt = nodes_.find(sourceId);
    auto targetIt = nodes_.find(targetId);
    
    if (sourceIt == nodes_.end() || targetIt == nodes_.end()) {
        throw std::runtime_error("Source or target node does not exist");
    }
    
    edges_.push_back(std::make_shared<Edge>(sourceIt->second, targetIt->second, weight));
}

void Graph::removeEdge(int sourceId, int targetId) {
    edges_.erase(
        std::remove_if(edges_.begin(), edges_.end(),
            [sourceId, targetId](const std::shared_ptr<Edge>& edge) {
                return edge->getSource()->getId() == sourceId && 
                       edge->getTarget()->getId() == targetId;
            }
        ),
        edges_.end()
    );
}

bool Graph::isDirected() const {
    return isDirected_;
}

std::vector<std::shared_ptr<Node>> Graph::getNodes() const {
    std::vector<std::shared_ptr<Node>> result;
    for (const auto& pair : nodes_) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::shared_ptr<Edge>> Graph::getEdges() const {
    return edges_;
}

void Graph::clear() {
    nodes_.clear();
    edges_.clear();
    nextNodeId_ = 0;
}

void Graph::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for writing");
    }
    
    // Write header
    file << (isDirected_ ? "directed" : "undirected") << "\n";
    
    // Write nodes
    file << nodes_.size() << "\n";
    for (const auto& pair : nodes_) {
        const auto& node = pair.second;
        file << node->getId() << " " << node->getX() << " " << node->getY() << "\n";
    }
    
    // Write edges
    file << edges_.size() << "\n";
    for (const auto& edge : edges_) {
        file << edge->getSource()->getId() << " " 
             << edge->getTarget()->getId() << " " 
             << edge->getWeight() << "\n";
    }
}

void Graph::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for reading");
    }
    
    clear();
    
    // Read header
    std::string type;
    file >> type;
    isDirected_ = (type == "directed");
    
    // Read nodes
    int nodeCount;
    file >> nodeCount;
    for (int i = 0; i < nodeCount; ++i) {
        int id;
        double x, y;
        file >> id >> x >> y;
        auto node = std::make_shared<Node>(id, x, y);
        nodes_[id] = node;
        nextNodeId_ = std::max(nextNodeId_, id + 1);
    }
    
    // Read edges
    int edgeCount;
    file >> edgeCount;
    for (int i = 0; i < edgeCount; ++i) {
        int sourceId, targetId;
        double weight;
        file >> sourceId >> targetId >> weight;
        addEdge(sourceId, targetId, weight);
    }
}
}