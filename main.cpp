#include <QApplication>
#include "MainWindow.hpp"
#include "Library.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Library library;
    try { library.loadData(); }
    catch (const std::exception& e) { std::cout << e.what() << std::endl; }

    MainWindow window(&library);
    window.show();

    return QApplication::exec();
}