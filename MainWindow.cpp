#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mCentralWidget = new CentralWidget;

    setCentralWidget(mCentralWidget);
}

MainWindow::~MainWindow() {}
