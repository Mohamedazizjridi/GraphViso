# Graphviso - Interactive Graph Algorithm Visualization

A C++/Qt application for creating, editing, and visualizing graph algorithms (BFS, DFS, Dijkstra).

## 🎯 Features

- Interactive graph creation (drag & drop nodes)
- Add/remove nodes and edges
- Directed and undirected graphs
- Algorithm visualization:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra's Shortest Path
- Step-by-step animation
- Save/Load graphs
- Weighted edges support

## 🛠️ Prerequisites

- C++ compiler with C++17 support
- Qt 6.x
- CMake 3.16 or higher

## 🚀 Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## 📝 Project Structure

```
graphviso/
├── include/            # Header files
│   ├── core/          # Core graph data structures
│   ├── algorithms/    # Graph algorithms implementation
│   ├── gui/           # Qt GUI components
│   └── utils/         # Utility functions
├── src/               # Source files
├── tests/             # Unit tests
├── resources/         # Application resources
# Graphviso - Interactive Graph Algorithm Visualization

Graphviso is a Qt-based educational tool to create and visualize graphs and common graph algorithms (DFS, BFS, Dijkstra).

## What we have

- Qt GUI with `MainWindow`, `NodeItem`, `EdgeItem`, and a `GraphInputDialog` for quick graph creation.
- Core model in `include/core`: `Graph`, `Node`, `Edge`.
- Algorithm visualizations:
  - DFS: step-by-step animation (yellow nodes), traverses all components; saved order via "Save DFS Order".
  - BFS: analogous animation (green nodes).
  - Dijkstra: interactive start/end selection; stepper-based relax visualization and final path highlight.
- Toolbar actions for building graphs and running visualizations.

## Quick start (build & run)

Install Qt6 development packages and CMake. Then:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
./src/graphviso
```

If the build fails with missing Qt headers, install the appropriate Qt development package for your distribution.

## Cleanup performed

- Removed generated build artifacts from the repo by cleaning the `build/` directory (do a fresh build as shown above).
- Added a `.gitignore` to keep build artifacts out of the repository.

## Suggested enhancements (priority)

1. Interactive edge creation: click source node then target node to add an edge with optional weight.
2. Playback controls: Play / Pause / Step / Speed slider for DFS/BFS/Dijkstra.
3. Separate export formats for algorithm outputs (DFS order, BFS order, Dijkstra path).
4. Unit tests for algorithms (headless mode) and a CI job to run builds/tests.
5. Scene editing improvements: select & delete nodes/edges, edit weights, and support for undo/redo.

## Next steps I can take for you

- Implement interactive edge creation (recommended next small UI feature).
- Add playback controls for traversals.
- Add unit tests and a CI workflow.

Tell me which of the next steps you'd like me to implement first and I'll start on it.

---
