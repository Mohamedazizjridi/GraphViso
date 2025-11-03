#include "core/edge.hpp"

namespace graphviso {

Edge::Edge(std::shared_ptr<Node> source, std::shared_ptr<Node> target, double weight)
    : source_(source), target_(target), weight_(weight) {}

std::shared_ptr<Node> Edge::getSource() const {
    return source_;
}

std::shared_ptr<Node> Edge::getTarget() const {
    return target_;
}

double Edge::getWeight() const {
    return weight_;
}

void Edge::setWeight(double weight) {
    weight_ = weight;
}

} // namespace graphviso