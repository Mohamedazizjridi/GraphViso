#pragma once
#include <bits/stdc++.h>
#include <memory>
#include "node.hpp"
using namespace std;
namespace graphviso {

class Edge {
public:
    Edge(shared_ptr<Node> source, shared_ptr<Node> target, double weight = 1.0);
    
    shared_ptr<Node> getSource() const;
    shared_ptr<Node> getTarget() const;
    double getWeight() const;
    void setWeight(double weight);

private:
    shared_ptr<Node> source_;
    shared_ptr<Node> target_;
    double weight_;
};

} // namespace graphviso