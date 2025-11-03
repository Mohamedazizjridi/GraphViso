#include "gui/node_item.hpp"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace graphviso {
namespace gui {

NodeItem::NodeItem(std::shared_ptr<Node> node, QGraphicsItem* parent)
    : QGraphicsItem(parent), node_(node), state_(State::Normal)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    
    setPos(node->getX(), node->getY());
}

QRectF NodeItem::boundingRect() const {
    return QRectF(-20, -20, 40, 40);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    // Draw node circle using state-aware color
    QColor fillColor = Qt::white;
    switch (state_) {
        case State::Normal: fillColor = Qt::white; break;
        case State::Current: fillColor = Qt::yellow; break;
        case State::Visited: fillColor = Qt::yellow; break; // DFS visited shown as yellow
        case State::BFSCurrent: fillColor = Qt::green; break;
        case State::BFSVisited: fillColor = Qt::green; break; // BFS visited shown as green
        case State::DCurrent: fillColor = QColor(0, 122, 204); break; // blue-ish
        case State::DVisited: fillColor = QColor(0, 102, 180); break;
    }
    painter->setBrush(fillColor);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(-15, -15, 30, 30);
    
    // Draw node ID
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(node_->getId()));
}

std::shared_ptr<Node> NodeItem::getNode() const {
    return node_;
}

// New state support
void NodeItem::setState(State s) {
    if (state_ != s) {
        state_ = s;
        update();
    }
}

NodeItem::State NodeItem::state() const {
    return state_;
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseMoveEvent(event);
    node_->setPosition(pos().x(), pos().y());
    
    // Notify edge items to update their positions
    scene()->update();
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
}

} // namespace gui
} // namespace graphviso