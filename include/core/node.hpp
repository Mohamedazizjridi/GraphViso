#pragma once
#include <bits/stdc++.h>
#include <memory>
using namespace std;
namespace graphviso {

class Node {
public:
    Node(int id, double x = 0, double y = 0);
    
    int getId() const;
    double getX() const;
    double getY() const;
    void setPosition(double x, double y);

private:
    int id_;
    double x_;
    double y_;
};

} // namespace graphviso