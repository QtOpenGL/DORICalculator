QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(Dependencies/Dependencies.pri)

SOURCES += \
    GUI/Base/AxisDrawer.cpp \
    GUI/Base/Handle.cpp \
    GUI/Widgets/CentralWidget.cpp \
    GUI/Widgets/SideViewWidget.cpp \
    GUI/Widgets/TopViewWidget.cpp \
    GUI/Widgets/ViewWidget.cpp \
    MainWindow.cpp \
    Main.cpp

HEADERS += \
    GUI/Base/AxisDrawer.h \
    GUI/Base/Handle.h \
    GUI/Widgets/CentralWidget.h \
    GUI/Widgets/SideViewWidget.h \
    GUI/Widgets/TopViewWidget.h \
    GUI/Widgets/ViewWidget.h \
    MainWindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
