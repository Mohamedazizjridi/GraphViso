#pragma once
#include <bits/stdc++.h>
using namespace std;
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QPair>

namespace graphviso {
namespace gui {

// Simple PODs for dialog results (keeps Qt-free model in core if needed)
struct DialogNode { int id; double x; double y; };
struct DialogEdge { int source; int target; double weight; };

class GraphInputDialog : public QDialog {
    Q_OBJECT
public:
    explicit GraphInputDialog(QWidget* parent = nullptr);

    // Accessors for the collected data
    vector<DialogNode> nodes() const;
    vector<DialogEdge> edges() const;

private slots:
    void addNode();
    void addEdge();

private:
    // Node inputs
    QLineEdit* nodeIdEdit_ = nullptr;
    QLineEdit* nodeXEdit_ = nullptr;
    QLineEdit* nodeYEdit_ = nullptr;
    QPushButton* addNodeBtn_ = nullptr;

    // Edge inputs
    QComboBox* edgeSrcCombo_ = nullptr;
    QComboBox* edgeTgtCombo_ = nullptr;
    QLineEdit* edgeWeightEdit_ = nullptr;
    QPushButton* addEdgeBtn_ = nullptr;

    // Done button
    QPushButton* doneBtn_ = nullptr;

    // Quick create
    QSpinBox* nodeCountSpin_ = nullptr;
    QPushButton* createNodesBtn_ = nullptr;
    QCheckBox* directedCheck_ = nullptr;

    vector<DialogNode> nodes_;
    vector<DialogEdge> edges_;
    bool directed_ = false;

public:
    bool isDirected() const { return directed_; }
};

} // namespace gui
} // namespace graphviso
