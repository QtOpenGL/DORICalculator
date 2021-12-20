#include <Core/Controller.h>
#include <GUI/CentralWidget.h>
#include <OpenGL/OpenGLWidget.h>

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QStyleFactory>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file("Resources/style.qss");
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
        font.setPixelSize(12);
        qApp->setFont(font);
        qDebug() << "Font is loaded. Using" << qApp->font();
    }

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    qDebug().noquote() << "QSurfaceFormat:" << QSurfaceFormat::defaultFormat();

    // Initialize
    Controller *controller = new Controller;
    controller->centralWidget()->showMaximized();
    controller->openGLWidget()->showNormal();

    return app.exec();
}
