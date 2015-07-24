#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("e-libatt");
    QCoreApplication::setOrganizationDomain("eli-batt.com");
    QCoreApplication::setApplicationName("elibatt-viewer");

    MainWindow w;

    w.show();

    return a.exec();
}
