#include "gui/main_window.hpp"
#include "gui/node_item.hpp"
#include "gui/edge_item.hpp"
#include "gui/graph_input_dialog.hpp"
#include <unordered_map>
#include <unordered_set>
#include "algorithms/traversal.hpp"
#include "algorithms/dijkstra.hpp"
#include "core/graph.hpp"
#include <set>
#include <QInputDialog>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <chrono>
#include <thread>

namespace graphviso {
namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), graph_(std::make_unique<Graph>())
{
    setupUI();
    setupToolbar();
}

void MainWindow::setupUI() {
    setWindowTitle("Graphviso - Graph Visualization");
    resize(800, 600);
    
    // Create a main widget and layout
    auto mainWidget = new QWidget(this);
    auto mainLayout = new QVBoxLayout(mainWidget);
    
    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(-400, -300, 800, 600);
    
    view_ = new QGraphicsView(scene_, this);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setDragMode(QGraphicsView::RubberBandDrag);
    view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    
    // Add status label
    statusLabel_ = new QLabel("Ready", this);
    statusLabel_->setAlignment(Qt::AlignCenter);
    
    mainLayout->addWidget(view_);
    mainLayout->addWidget(statusLabel_);
    
    setCentralWidget(mainWidget);
}

void MainWindow::setupToolbar() {
    auto toolbar = addToolBar("Tools");
    
    auto newAction = toolbar->addAction("New");
    connect(newAction, &QAction::triggered, this, &MainWindow::newGraph);

    auto buildAction = toolbar->addAction("Build Graph");
    connect(buildAction, &QAction::triggered, this, &MainWindow::buildGraph);
    
    auto openAction = toolbar->addAction("Open");
    connect(openAction, &QAction::triggered, this, &MainWindow::openGraph);
    
    auto saveAction = toolbar->addAction("Save");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveGraph);
    
    toolbar->addSeparator();
    
    auto directedAction = toolbar->addAction("Directed");
    directedAction->setCheckable(true);
    connect(directedAction, &QAction::toggled, this, &MainWindow::toggleDirected);
    
    toolbar->addSeparator();
    
    auto bfsAction = toolbar->addAction("Run BFS");
    connect(bfsAction, &QAction::triggered, this, &MainWindow::runBFS);
    
    auto dfsAction = toolbar->addAction("Run DFS");
    connect(dfsAction, &QAction::triggered, this, &MainWindow::runDFS);
    
    auto saveDfsAction = toolbar->addAction("Save DFS Order");
    connect(saveDfsAction, &QAction::triggered, this, &MainWindow::saveDFSOrder);
    
    auto dijkstraAction = toolbar->addAction("Run Dijkstra");
    connect(dijkstraAction, &QAction::triggered, this, &MainWindow::runDijkstra);
}

void MainWindow::newGraph() {
    graph_->clear();
    scene_->clear();

    // Create nodes
    auto node1Data = std::make_shared<Node>(1, -100, 0);
    auto node2Data = std::make_shared<Node>(2, 0, 100);
    auto node3Data = std::make_shared<Node>(3, 100, 0);

    auto node1 = new NodeItem(node1Data);
    auto node2 = new NodeItem(node2Data);
    auto node3 = new NodeItem(node3Data);

    node1->setPos(node1Data->getX(), node1Data->getY());
    node2->setPos(node2Data->getX(), node2Data->getY());
    node3->setPos(node3Data->getX(), node3Data->getY());

    scene_->addItem(node1);
    scene_->addItem(node2);
    scene_->addItem(node3);

    // Create edges
    auto edge1Data = std::make_shared<Edge>(node1Data, node2Data);
    auto edge2Data = std::make_shared<Edge>(node2Data, node3Data);
    auto edge3Data = std::make_shared<Edge>(node3Data, node1Data);

    auto edge1 = new EdgeItem(edge1Data);
    auto edge2 = new EdgeItem(edge2Data);
    auto edge3 = new EdgeItem(edge3Data);

    scene_->addItem(edge1);
    scene_->addItem(edge2);
    scene_->addItem(edge3);
}

void MainWindow::openGraph() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Graph", "", "Graph Files (*.graph)");
    if (filename.isEmpty()) {
        return;
    }
    
    try {
        graph_->load(filename.toStdString());
        // TODO: Update visualization
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load graph: %1").arg(e.what()));
    }
}

void MainWindow::buildGraph() {
    GraphInputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        // Recreate graph with directed flag
        graph_ = std::make_unique<Graph>(dlg.isDirected());
        auto nodes = dlg.nodes();
        auto edges = dlg.edges();
        buildGraphFromInput(nodes, edges);
    }
}

void MainWindow::buildGraphFromInput(const std::vector<graphviso::gui::DialogNode>& nodes,
                                    const std::vector<graphviso::gui::DialogEdge>& edges) {
    graph_->clear();
    scene_->clear();

    // Map user-provided ID -> actual Graph node id
    std::unordered_map<int, int> userToGraphId;
    std::unordered_map<int, NodeItem*> nodeItemMap;

    for (const auto& n : nodes) {
        auto created = graph_->addNode(n.x, n.y); // Graph assigns real ID
        userToGraphId[n.id] = created->getId();
        auto ni = new NodeItem(created);
        ni->setPos(n.x, n.y);
        scene_->addItem(ni);
        nodeItemMap[n.id] = ni;
    }

    for (const auto& e : edges) {
        auto sit = userToGraphId.find(e.source);
        auto tit = userToGraphId.find(e.target);
        if (sit == userToGraphId.end() || tit == userToGraphId.end()) continue;
        int realSrc = sit->second;
        int realTgt = tit->second;
        graph_->addEdge(realSrc, realTgt, e.weight);
        // find NodeItem pointers to create EdgeItem visual
        NodeItem* srcItem = nodeItemMap[e.source];
        NodeItem* tgtItem = nodeItemMap[e.target];
        if (srcItem && tgtItem) {
            // Create EdgeItem using underlying Edge object from graph
            auto ed = std::make_shared<Edge>(srcItem->getNode(), tgtItem->getNode(), e.weight);
            auto ei = new EdgeItem(ed);
            scene_->addItem(ei);
        }
    }
}

void MainWindow::saveGraph() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Graph", "", "Graph Files (*.graph)");
    if (filename.isEmpty()) {
        return;
    }
    
    try {
        graph_->save(filename.toStdString());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to save graph: %1").arg(e.what()));
    }
}

void MainWindow::runBFS() {
    // Ensure the underlying graph has nodes
    auto nodes = graph_->getNodes();
    if (nodes.empty()) {
        QMessageBox::information(this, "BFS", "Graph is empty. Create nodes first.");
        return;
    }

    // Build mapping from node id -> NodeItem (visual)
    std::unordered_map<int, NodeItem*> idToNode;
    for (QGraphicsItem* item : scene_->items()) {
        if (auto ni = dynamic_cast<NodeItem*>(item)) {
            idToNode[ni->getNode()->getId()] = ni;
            ni->setState(NodeItem::State::Normal);
        }
    }

    // Clear previous BFS order
    lastDfsOrder_.clear();

    // Run BFS across all connected components
    algorithms::BFS bfs;
    std::vector<int> order;
    std::unordered_set<int> visitedIds;

    for (const auto& nptr : nodes) {
        int nid = nptr->getId();
        if (visitedIds.count(nid)) continue;
        bfs.traverse(*graph_, nid, [&](const graphviso::Node& n) {
            int id = n.getId();
            if (!visitedIds.count(id)) {
                visitedIds.insert(id);
                order.push_back(id);
            }
        });
    }

    if (order.empty()) {
        QMessageBox::warning(this, "BFS", "No nodes visited by BFS.");
        return;
    }

    // Store the order for later saving (reuse lastDfsOrder_ storage)
    lastDfsOrder_ = order;

    // Animate highlights using QTimer singleShot: mark each visited node as BFSCurrent, then BFSVisited
    int delayMs = 1000; // 1 second between steps for visibility
    for (size_t i = 0; i < order.size(); ++i) {
        int nodeId = order[i];
        QTimer::singleShot(static_cast<int>(i) * delayMs, this,
            [this, nodeId, i, total = order.size(), idToNode, order = order, delayMs]() {
            // Set previous node to BFSVisited state if not the first node
            if (i > 0) {
                int prevId = order[i-1];
                if (auto prevNode = idToNode.at(prevId)) {
                    prevNode->setState(NodeItem::State::BFSVisited);
                }
            }

            // Set current node to BFSCurrent state (green)
            if (auto currentNode = idToNode.at(nodeId)) {
                currentNode->setState(NodeItem::State::BFSCurrent);
                statusLabel_->setText(QString("BFS: Visiting node %1 (%2 of %3)").arg(nodeId).arg(i + 1).arg(total));
            }

            // If this is the last node, set it to BFSVisited after a delay
            if (i == total - 1) {
                QTimer::singleShot(delayMs, this, [this, nodeId, idToNode]() {
                    if (auto lastNode = idToNode.at(nodeId)) {
                        lastNode->setState(NodeItem::State::BFSVisited);
                        statusLabel_->setText("BFS traversal complete. Use 'Save DFS Order' to save the results.");
                    }
                });
            }
        });
    }
}

void MainWindow::runDFS() {
    // Ensure the underlying graph has nodes
    auto nodes = graph_->getNodes();
    if (nodes.empty()) {
        QMessageBox::information(this, "DFS", "Graph is empty. Create nodes first.");
        return;
    }

    // Build mapping from node id -> NodeItem (visual)
    std::unordered_map<int, NodeItem*> idToNode;
    for (QGraphicsItem* item : scene_->items()) {
        if (auto ni = dynamic_cast<NodeItem*>(item)) {
            idToNode[ni->getNode()->getId()] = ni;
            ni->setState(NodeItem::State::Normal);
        }
    }

    // Clear previous DFS order
    lastDfsOrder_.clear();

    // Run DFS on the actual graph_ (core::Graph) across all connected components
    algorithms::DFS dfs;
    std::vector<int> order;
    std::unordered_set<int> visitedIds;

    // For every node in the graph, start a DFS if it hasn't been visited yet.
    for (const auto& nptr : nodes) {
        int nid = nptr->getId();
        if (visitedIds.count(nid)) continue;
        dfs.traverse(*graph_, nid, [&](const graphviso::Node& n) {
            int id = n.getId();
            if (!visitedIds.count(id)) {
                visitedIds.insert(id);
                order.push_back(id);
            }
        });
    }

    if (order.empty()) {
        QMessageBox::warning(this, "DFS", "No nodes visited by DFS.");
        return;
    }

    // Store the order for later saving
    lastDfsOrder_ = order;

    // Animate highlights using QTimer singleShot: mark each visited node as Current, then Visited
    int delayMs = 1000; // 1 second between steps for better visibility
    for (size_t i = 0; i < order.size(); ++i) {
        int nodeId = order[i];
        // Capture all needed variables by value except 'this'
        QTimer::singleShot(static_cast<int>(i) * delayMs, this, 
            [this, nodeId, i, total = order.size(), idToNode, order = order, delayMs]() {
            // Set previous node to Visited state if not the first node
            if (i > 0) {
                int prevId = order[i-1];
                if (auto prevNode = idToNode.at(prevId)) {
                    prevNode->setState(NodeItem::State::Visited);
                }
            }

            // Set current node to Current state (yellow)
            if (auto currentNode = idToNode.at(nodeId)) {
                currentNode->setState(NodeItem::State::Current);
                statusLabel_->setText(QString("DFS: Visiting node %1 (%2 of %3)").arg(nodeId).arg(i + 1).arg(total));
            }

            // If this is the last node, set it to Visited after a delay
            if (i == total - 1) {
                QTimer::singleShot(delayMs, this, 
                    [this, nodeId, idToNode]() {
                    if (auto lastNode = idToNode.at(nodeId)) {
                        lastNode->setState(NodeItem::State::Visited);
                        statusLabel_->setText("DFS traversal complete. Use 'Save DFS Order' to save the results.");
                    }
                });
            }
        });
    }
    }

// loadDFS was removed per UX request: saving is handled by saveDFSOrder() and replay isn't needed.

void MainWindow::runDijkstra() {
    // Ask user to select start and end nodes
    auto nodes = graph_->getNodes();
    if (nodes.empty()) {
        QMessageBox::information(this, "Dijkstra", "Graph is empty. Create nodes first.");
        return;
    }

    QStringList idList;
    for (const auto& n : nodes) idList << QString::number(n->getId());

    bool ok = false;
    QString startStr = QInputDialog::getItem(this, "Dijkstra", "Start node:", idList, 0, false, &ok);
    if (!ok || startStr.isEmpty()) return;
    QString endStr = QInputDialog::getItem(this, "Dijkstra", "End node:", idList, 0, false, &ok);
    if (!ok || endStr.isEmpty()) return;

    int startId = startStr.toInt();
    int endId = endStr.toInt();

    // Build adjacency list and visual mapping
    std::unordered_map<int, std::vector<std::pair<int,double>>> adj;
    std::unordered_map<int, NodeItem*> idToNode;
    std::vector<EdgeItem*> edgeItems;
    for (QGraphicsItem* it : scene_->items()) {
        if (auto ei = dynamic_cast<EdgeItem*>(it)) {
            edgeItems.push_back(ei);
            auto ed = ei->getEdge();
            int s = ed->getSource()->getId();
            int t = ed->getTarget()->getId();
            double w = ed->getWeight();
            adj[s].push_back({t,w});
            if (!graph_->isDirected()) adj[t].push_back({s,w});
        }
        if (auto ni = dynamic_cast<NodeItem*>(it)) {
            idToNode[ni->getNode()->getId()] = ni;
            ni->setState(NodeItem::State::Normal);
        }
    }

    // Prepare Dijkstra structures
    auto dist = std::make_shared<std::unordered_map<int,double>>();
    auto prev = std::make_shared<std::unordered_map<int,int>>();
    std::unordered_set<int> visited;

    const double INF = std::numeric_limits<double>::infinity();
    for (const auto& n : nodes) (*dist)[n->getId()] = INF;
    (*dist)[startId] = 0.0;

    // Use set as priority queue (distance, id)
    auto active = std::make_shared<std::set<std::pair<double,int>>>();
    active->insert({0.0, startId});

    // Timer-based stepper
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (active->empty()) {
            // finished, reconstruct path
            timer->stop();
            timer->deleteLater();
            // build path
            std::vector<int> path;
            int cur = endId;
            if (prev->find(cur) == prev->end() && cur != startId) {
                statusLabel_->setText("No path found.");
                return;
            }
            while (cur != startId) {
                path.push_back(cur);
                cur = (*prev)[cur];
            }
            path.push_back(startId);
            std::reverse(path.begin(), path.end());

            // Highlight path edges and nodes
            for (size_t i = 0; i + 1 < path.size(); ++i) {
                int a = path[i], b = path[i+1];
                // highlight node
                if (idToNode.count(a)) idToNode[a]->setState(NodeItem::State::DVisited);
                if (idToNode.count(b)) idToNode[b]->setState(NodeItem::State::DVisited);
                // highlight edge
                for (auto ei : edgeItems) {
                    auto ed = ei->getEdge();
                    if ((ed->getSource()->getId() == a && ed->getTarget()->getId() == b) ||
                        (!graph_->isDirected() && ed->getSource()->getId() == b && ed->getTarget()->getId() == a)) {
                        ei->highlight(true);
                    }
                }
            }

            statusLabel_->setText("Dijkstra complete. Path highlighted.");
            return;
        }

        // pop smallest
        auto it = active->begin();
        auto [d, u] = *it;
        active->erase(it);
        if (visited.count(u)) return; // already processed
        visited.insert(u);

        // mark current
        if (idToNode.count(u)) idToNode[u]->setState(NodeItem::State::DCurrent);

        // relax neighbors
        for (auto [v,w] : adj[u]) {
            if (visited.count(v)) continue;
            double nd = (*dist)[u] + w;
            if (nd < (*dist)[v]) {
                // remove old entry if exists
                if ((*dist)[v] != INF) {
                    active->erase({(*dist)[v], v});
                }
                (*dist)[v] = nd;
                (*prev)[v] = u;
                active->insert({nd, v});

                // highlight the edge being relaxed briefly
                for (auto ei : edgeItems) {
                    auto ed = ei->getEdge();
                    if ((ed->getSource()->getId() == u && ed->getTarget()->getId() == v) ||
                        (!graph_->isDirected() && ed->getSource()->getId() == v && ed->getTarget()->getId() == u)) {
                        ei->highlight(true);
                        QTimer::singleShot(300, this, [ei]() { ei->highlight(false); });
                    }
                }
            }
        }

        // set u to visited state after processing
        if (idToNode.count(u)) idToNode[u]->setState(NodeItem::State::DVisited);
    });

    statusLabel_->setText("Starting Dijkstra...");
    timer->start(300);
}

void MainWindow::toggleDirected() {
    // TODO: Implement directed/undirected toggle
}

void MainWindow::saveDFSOrder() {
    if (lastDfsOrder_.empty()) {
        QMessageBox::warning(this, "Save DFS Order", "No DFS traversal has been performed yet.");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, 
        "Save DFS Order", 
        "dfs_order.txt", 
        "Text Files (*.txt);;All Files (*)");

    if (filename.isEmpty()) {
        return;
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int id : lastDfsOrder_) {
            out << id << '\n';
        }
        file.close();
        statusLabel_->setText("DFS order saved successfully.");
    } else {
        QMessageBox::warning(this, "Save Failed", "Could not open file for writing.");
    }
}

} // namespace gui
} // namespace graphviso