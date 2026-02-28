#include <QApplication>
#include <QIcon>
#include "MainWindow.hpp"
#include "Library.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/app_icon.ico"));

    Library library;
    try {
        library.loadData();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    MainWindow window(&library);
    window.show();

    return QApplication::exec();
}