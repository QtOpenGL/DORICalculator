#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("StyleSheet.qss");

    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "Could not find style sheet. Using the default one.";
    }

    MainWindow w;
    w.setMinimumSize(640, 480);
    w.showMaximized();
    return a.exec();
}
