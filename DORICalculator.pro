QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(Dependencies/Dependencies.pri)

SOURCES += \
    MainWindow.cpp \
    Main.cpp \
    Widgets/CentralWidget.cpp \
    Widgets/SideViewWidget.cpp \
    Widgets/TopViewWidget.cpp

HEADERS += \
    MainWindow.h \
    Parameters.h \
    Widgets/CentralWidget.h \
    Widgets/SideViewWidget.h \
    Widgets/TopViewWidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
