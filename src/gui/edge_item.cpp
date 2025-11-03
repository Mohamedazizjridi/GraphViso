#include "gui/edge_item.hpp"
#include "gui/node_item.hpp"
#include <QPainter>
#include <QGraphicsScene>
#include <cmath>

namespace graphviso {
namespace gui {

EdgeItem::EdgeItem(std::shared_ptr<Edge> edge, QGraphicsItem* parent)
    : QGraphicsItem(parent), edge_(edge), isHighlighted_(false)
{
    setZValue(-1); // Draw edges behind nodes
}

QRectF EdgeItem::boundingRect() const {
    auto source = edge_->getSource();
    auto target = edge_->getTarget();
    
    return QRectF(
        std::min(source->getX(), target->getX()) - 20,
        std::min(source->getY(), target->getY()) - 20,
        std::abs(source->getX() - target->getX()) + 40,
        std::abs(source->getY() - target->getY()) + 40
    );
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    auto source = edge_->getSource();
    auto target = edge_->getTarget();
    
    QPointF sourcePoint(source->getX(), source->getY());
    QPointF targetPoint(target->getX(), target->getY());
    
    // Draw the line
    painter->setPen(QPen(isHighlighted_ ? Qt::red : Qt::black, 2));
    painter->drawLine(sourcePoint, targetPoint);
    
    // Draw the weight
    QPointF midPoint = (sourcePoint + targetPoint) / 2;
    painter->drawText(midPoint, QString::number(edge_->getWeight()));
}

std::shared_ptr<Edge> EdgeItem::getEdge() const {
    return edge_;
}

void EdgeItem::highlight(bool enabled) {
    if (isHighlighted_ != enabled) {
        isHighlighted_ = enabled;
        update();
    }
}

void EdgeItem::updatePosition() {
    prepareGeometryChange();
}

} // namespace gui
} // namespace graphviso