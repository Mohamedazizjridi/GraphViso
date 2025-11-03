#pragma once

#include <bits/stdc++.h>
#include <memory>
#include "node.hpp"
#include "edge.hpp"
using namespace std;

namespace graphviso {

class Graph {
public:
    Graph(bool isDirected = false);
    
    shared_ptr<Node> addNode(double x, double y);
    void removeNode(int id);
    void addEdge(int sourceId, int targetId, double weight = 1.0);
    void removeEdge(int sourceId, int targetId);
    
    bool isDirected() const;
    vector<shared_ptr<Node>> getNodes() const;
    vector<shared_ptr<Edge>> getEdges() const;
    
    void clear();
    void save(const string& filename) const;
    void load(const string& filename);

private:
    bool isDirected_;
    unordered_map<int, shared_ptr<Node>> nodes_;
    vector<shared_ptr<Edge>> edges_;
    int nextNodeId_;
};

} // namespace graphviso