#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(640, 480);
    w.show();
    return a.exec();
}

//#include <iostream>
//#include <Dependencies/Eigen/Dense>

//using Eigen::MatrixXd;

//int main()
//{
//  MatrixXd m(2,2);
//  m(0,0) = 3;
//  m(1,0) = 2.5;
//  m(0,1) = -1;
//  m(1,1) = m(1,0) + m(0,1);
//  std::cout << m << std::endl;
//}
