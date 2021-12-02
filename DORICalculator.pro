QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(Dependencies/Dependencies.pri)

SOURCES += \
    Core/Controller.cpp \
    Core/Logic.cpp \
    GUI/AxisWidget.cpp \
    GUI/CentralWidget.cpp \
    GUI/Handle.cpp \
    GUI/LeftWidget.cpp \
    GUI/SideViewWidget.cpp \
    GUI/TopViewWidget.cpp \
    Main.cpp \
    OpenGL/Camera.cpp \
    OpenGL/Data/ObjectData.cpp \
    OpenGL/Data/RegionData.cpp \
    OpenGL/Light.cpp \
    OpenGL/Object/Object.cpp \
    OpenGL/OpenGLWidget.cpp \
    OpenGL/Renderer/LineRenderer.cpp \
    OpenGL/Renderer/ObjectRenderer.cpp \
    OpenGL/Renderer/RegionRenderer.cpp

HEADERS += \
    Core/Constants.h \
    Core/Controller.h \
    Core/Enums.h \
    Core/Logic.h \
    Core/Typedefs.h \
    GUI/AxisWidget.h \
    GUI/CentralWidget.h \
    GUI/Handle.h \
    GUI/LeftWidget.h \
    GUI/SideViewWidget.h \
    GUI/TopViewWidget.h \
    OpenGL/Camera.h \
    OpenGL/Data/ObjectData.h \
    OpenGL/Data/RegionData.h \
    OpenGL/Light.h \
    OpenGL/Object/Object.h \
    OpenGL/OpenGLWidget.h \
    OpenGL/Renderer/LineRenderer.h \
    OpenGL/Renderer/ObjectRenderer.h \
    OpenGL/Renderer/RegionRenderer.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Shaders/Line/FragmentShader.frag \
    Shaders/Line/VertexShader.vert \
    Shaders/Object/FragmentShader.frag \
    Shaders/Object/VertexShader.vert \
    Shaders/Region/FragmentShader.frag \
    Shaders/Region/VertexShader.vert


