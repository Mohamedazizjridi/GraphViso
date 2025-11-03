add_library(graphviso_core
    core/node.cpp
    core/edge.cpp
    core/graph.cpp
)
set_target_properties(graphviso_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(graphviso_algorithms
    algorithms/traversal.cpp
    algorithms/dijkstra.cpp
)
set_target_properties(graphviso_algorithms PROPERTIES POSITION_INDEPENDENT_CODE ON)

qt_wrap_cpp(MOC_SOURCES ../include/gui/main_window.hpp ../include/gui/graph_input_dialog.hpp)

add_library(graphviso_gui
    gui/node_item.cpp
    gui/edge_item.cpp
    gui/main_window.cpp
    gui/graph_input_dialog.cpp
    ${MOC_SOURCES}
)
set_target_properties(graphviso_gui PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(graphviso main.cpp)
set_target_properties(graphviso PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(graphviso_gui PRIVATE
    Qt6::Core
    Qt6::Widgets
    graphviso_core
    graphviso_algorithms
)

target_link_libraries(graphviso_algorithms PRIVATE
    graphviso_core
)

target_link_libraries(graphviso PRIVATE
    graphviso_gui
    graphviso_core
    graphviso_algorithms
    Qt6::Core
    Qt6::Widgets)