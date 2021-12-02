#ifndef LINERENDERER_H
#define LINERENDERER_H

#include <Core/Typedefs.h>
#include <OpenGL/Camera.h>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

class LineRenderer : protected QOpenGLFunctions
{
public:
    LineRenderer();

    bool init();
    void render(const QVector<QVector3D> &vertices, const Camera *camera);

protected:
    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    QOpenGLShaderProgram *mShader;

    Location mViewMatrix;
    Location mProjectionMatrix;

    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;

    bool mInit;
};

#endif // LINERENDERER_H
