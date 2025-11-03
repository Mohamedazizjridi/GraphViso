#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include "../core/graph.hpp"
// Dialog types used in buildGraphFromInput
#include "gui/graph_input_dialog.hpp"

namespace graphviso {
namespace gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void newGraph();
    void buildGraph();
    void openGraph();
    void saveGraph();
    void runBFS();
    void runDFS();
    void runDijkstra();
    void toggleDirected();
    void saveDFSOrder();

private:
    void setupUI();
    void setupToolbar();
    
    void setupGraphicsView();
    // Build a graph from dialog input
    void buildGraphFromInput(const std::vector<graphviso::gui::DialogNode>& nodes,
                             const std::vector<graphviso::gui::DialogEdge>& edges);
    
    QGraphicsScene* scene_;
    QGraphicsView* view_;
    std::unique_ptr<Graph> graph_;
    QLabel* statusLabel_{nullptr};
    std::vector<int> lastDfsOrder_; // Store last DFS order
};

} // namespace gui
} // namespace graphviso