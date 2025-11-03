#pragma once
#include <bits/stdc++.h>
using namespace std;
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
    void buildGraphFromInput(const vector<graphviso::gui::DialogNode>& nodes,
                             const vector<graphviso::gui::DialogEdge>& edges);
    
    QGraphicsScene* scene_;
    QGraphicsView* view_;
    unique_ptr<Graph> graph_;
    QLabel* statusLabel_{nullptr};
    vector<int> lastDfsOrder_; // Store last DFS order
};

} // namespace gui
} // namespace graphviso