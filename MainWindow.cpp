#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mController = new Dori::Core::Controller;
    mController->init();

    mCentralWidget = mController->centralWidget();

    setCentralWidget(mCentralWidget);
}

MainWindow::~MainWindow() {}
