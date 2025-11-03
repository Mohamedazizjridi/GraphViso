#include "core/node.hpp"

namespace graphviso {

Node::Node(int id, double x, double y)
    : id_(id), x_(x), y_(y) {}

int Node::getId() const {
    return id_;
}

double Node::getX() const {
    return x_;
}

double Node::getY() const {
    return y_;
}

void Node::setPosition(double x, double y) {
    x_ = x;
    y_ = y;
}

} // namespace graphviso
