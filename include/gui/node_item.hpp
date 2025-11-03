#pragma once

#include <QGraphicsItem>
#include "../core/node.hpp"

namespace graphviso {
namespace gui {

class NodeItem : public QGraphicsItem {
public:
    explicit NodeItem(std::shared_ptr<Node> node, QGraphicsItem* parent = nullptr);
    
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    
    std::shared_ptr<Node> getNode() const;
    enum class State { Normal, Current, Visited, BFSCurrent, BFSVisited, DCurrent, DVisited };
    void setState(State s);
    State state() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    std::shared_ptr<Node> node_;
    State state_ = State::Normal;
};

} // namespace gui
} // namespace graphviso