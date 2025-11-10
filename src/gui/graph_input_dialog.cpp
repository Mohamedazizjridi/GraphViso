#include "gui/graph_input_dialog.hpp"
#include <QLabel>
#include <QFormLayout>
#include <QMessageBox>
#include <bits/stdc++.h>
using namespace std;
using namespace graphviso::gui;

GraphInputDialog::GraphInputDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Build Graph");

    auto mainLayout = new QVBoxLayout(this);
    auto quickGroup = new QHBoxLayout();
    quickGroup->addWidget(new QLabel("Nodes:"));
    nodeCountSpin_ = new QSpinBox();
    nodeCountSpin_->setMinimum(1);
    nodeCountSpin_->setMaximum(1000);
    nodeCountSpin_->setValue(5);
    quickGroup->addWidget(nodeCountSpin_);

    directedCheck_ = new QCheckBox("Directed");
    quickGroup->addWidget(directedCheck_);

    createNodesBtn_ = new QPushButton("Create Nodes");
    quickGroup->addWidget(createNodesBtn_);
    connect(createNodesBtn_, &QPushButton::clicked, this, [this]() {
        int count = nodeCountSpin_->value();
        nodes_.clear();
        double radius = 150.0;
        double cx = 0, cy = 0;
        for (int i = 0; i < count; ++i) {
            double angle = (2.0 * M_PI * i) / count;
            double x = cx + radius * cos(angle);
            double y = cy + radius * sin(angle);
            nodes_.push_back(DialogNode{ i+1, x, y });
        }
        directed_ = directedCheck_->isChecked();
        QMessageBox::information(this, "Nodes Created", QString("Created %1 nodes.").arg(count));
    });

    mainLayout->addLayout(quickGroup);
    auto edgeGroup = new QVBoxLayout();
    edgeGroup->addWidget(new QLabel("Add Edge"));

    auto edgeForm = new QFormLayout();
    edgeSrcCombo_ = new QComboBox();
    edgeTgtCombo_ = new QComboBox();
    edgeWeightEdit_ = new QLineEdit();
    edgeForm->addRow("Source ID:", edgeSrcCombo_);
    edgeForm->addRow("Target ID:", edgeTgtCombo_);
    edgeForm->addRow("Weight:", edgeWeightEdit_);
    edgeGroup->addLayout(edgeForm);

    addEdgeBtn_ = new QPushButton("Add Edge");
    edgeGroup->addWidget(addEdgeBtn_);
    connect(addEdgeBtn_, &QPushButton::clicked, this, &GraphInputDialog::addEdge);
    connect(createNodesBtn_, &QPushButton::clicked, this, [this]() {
        edgeSrcCombo_->clear();
        edgeTgtCombo_->clear();
        for (const auto& n : nodes_) {
            QString id = QString::number(n.id);
            edgeSrcCombo_->addItem(id);
            edgeTgtCombo_->addItem(id);
        }
    });

    mainLayout->addLayout(edgeGroup);
    doneBtn_ = new QPushButton("Done");
    connect(doneBtn_, &QPushButton::clicked, this, &GraphInputDialog::accept);
    mainLayout->addWidget(doneBtn_);

    setLayout(mainLayout);
}

std::vector<DialogNode> GraphInputDialog::nodes() const {
    return nodes_;
}

std::vector<DialogEdge> GraphInputDialog::edges() const {
    return edges_;
}

void GraphInputDialog::addNode() {
    bool okId = false; double x = 0, y = 0;
    int id = nodeIdEdit_->text().toInt(&okId);
    if (!okId) {
        QMessageBox::warning(this, "Invalid ID", "Node ID must be an integer");
        return;
    }

    bool okX = false, okY = false;
    x = nodeXEdit_->text().toDouble(&okX);
    y = nodeYEdit_->text().toDouble(&okY);
    if (!okX || !okY) {
        QMessageBox::warning(this, "Invalid Position", "X and Y must be numbers");
        return;
    }
    for (const auto& n : nodes_) if (n.id == id) {
        QMessageBox::warning(this, "Duplicate ID", "A node with this ID already exists");
        return;
    }

    nodes_.push_back(DialogNode{ id, x, y });
    nodeIdEdit_->clear(); nodeXEdit_->clear(); nodeYEdit_->clear();
}

void GraphInputDialog::addEdge() {
    if (!edgeSrcCombo_ || !edgeTgtCombo_) return;
    bool okW = false;
    int src = edgeSrcCombo_->currentText().toInt();
    int tgt = edgeTgtCombo_->currentText().toInt();
    double w = edgeWeightEdit_->text().toDouble(&okW);
    if (!okW) w = 1.0;
    auto exists = [&](int id){ for (const auto& n: nodes_) if (n.id == id) return true; return false; };
    if (!exists(src) || !exists(tgt)) {
        QMessageBox::warning(this, "Unknown Node", "Source and target nodes must be added before creating an edge");
        return;
    }
    edges_.push_back(DialogEdge{ src, tgt, w });
    edgeWeightEdit_->clear();
}
