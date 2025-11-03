#include <QApplication>
#include <QWidget>
#include "gui/main_window.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    graphviso::gui::MainWindow window;
    window.show();
    
    return app.exec();
}