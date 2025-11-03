#pragma once

#include <QGraphicsItem>
#include "../core/edge.hpp"

namespace graphviso {
namespace gui {

class EdgeItem : public QGraphicsItem {
public:
    explicit EdgeItem(std::shared_ptr<Edge> edge, QGraphicsItem* parent = nullptr);
    
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    
    std::shared_ptr<Edge> getEdge() const;
    void highlight(bool enabled = true);
    void updatePosition();

private:
    std::shared_ptr<Edge> edge_;
    bool isHighlighted_;
};

} // namespace gui
} // namespace graphviso