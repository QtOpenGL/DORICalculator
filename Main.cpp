#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setMinimumSize(640, 480);
    w.showMaximized();
    return a.exec();
}
