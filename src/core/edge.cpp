#include "core/edge.hpp"
#include<bits/stdc++.h>
using namespace std;
namespace graphviso {

Edge::Edge(shared_ptr<Node> source, shared_ptr<Node> target, double weight)
    : source_(source), target_(target), weight_(weight) {}

shared_ptr<Node> Edge::getSource() const {
    return source_;
}

shared_ptr<Node> Edge::getTarget() const {
    return target_;
}

double Edge::getWeight() const {
    return weight_;
}

void Edge::setWeight(double weight) {
    weight_ = weight;
}

} // namespace graphviso
