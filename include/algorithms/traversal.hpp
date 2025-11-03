#pragma once

#include <vector>
#include <functional>
#include "../core/graph.hpp"

namespace graphviso {
namespace algorithms {

class GraphTraversal {
public:
    using VisitCallback = std::function<void(const Node&)>;
    
    virtual void traverse(const Graph& graph, int startNodeId, VisitCallback callback) = 0;
    virtual ~GraphTraversal() = default;
};

class BFS : public GraphTraversal {
public:
    void traverse(const Graph& graph, int startNodeId, VisitCallback callback) override;
};

class DFS : public GraphTraversal {
public:
    void traverse(const Graph& graph, int startNodeId, VisitCallback callback) override;
};

} // namespace algorithms
} // namespace graphviso