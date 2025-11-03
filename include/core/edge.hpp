#pragma once

#include <vector>
#include <memory>
#include "node.hpp"

namespace graphviso {

class Edge {
public:
    Edge(std::shared_ptr<Node> source, std::shared_ptr<Node> target, double weight = 1.0);
    
    std::shared_ptr<Node> getSource() const;
    std::shared_ptr<Node> getTarget() const;
    double getWeight() const;
    void setWeight(double weight);

private:
    std::shared_ptr<Node> source_;
    std::shared_ptr<Node> target_;
    double weight_;
};

} // namespace graphviso