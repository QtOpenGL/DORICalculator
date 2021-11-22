#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GUI/Widgets/CentralWidget.h"
#include <Core/Controller.h>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Controller *mController;
    CentralWidget *mCentralWidget;
};
#endif // MAINWINDOW_H
