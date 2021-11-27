#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("Resources/StyleSheet.qss");

    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "Could not find style sheet. Using the default one.";
    }

    int id = QFontDatabase::addApplicationFont("Resources/Fonts/Ubuntu/Ubuntu-Regular.ttf");
    if (id == -1) {
        QFont font = qApp->font();
        font.setPixelSize(12);
        font.setBold(false);
        qApp->setFont(font);
        qDebug() << "Font cannot be loaded. Using deafult font:" << qApp->font();
    } else {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont font(family);
        font.setPixelSize(13);
        qApp->setFont(font);
        qDebug() << "Font is loaded. Using" << qApp->font();
    }

    MainWindow w;
    w.showMaximized();
    return a.exec();
}
