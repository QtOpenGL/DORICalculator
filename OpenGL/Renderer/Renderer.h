#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGL/Camera.h>
#include <OpenGL/Light.h>
#include <OpenGL/Object/Object.h>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QString>

class Renderer : protected QOpenGLFunctions
{
public:
    Renderer(const QString &vertexShaderPath, const QString &fragmentShaderPath);
    virtual ~Renderer();
    virtual void render(QVector<Object *> objects, const Camera *camera, const Light *light) = 0;

protected:
    virtual bool initialize();
    virtual const float *getVertexData() const = 0;
    virtual const int getVertexDataByteSize() const = 0;

    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    QOpenGLShaderProgram *mShader;

    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;

    int mModelMatrixLocation;
    int mViewMatrixLocation;
    int mProjectionMatrixLocation;
    int mObjectColorLocation;
    int mAmbientStrengthLocation;
    int mCameraPositionLocation;
    int mLightPositionLocation;
    int mLightColorLocation;
    int mLightPowerLocation;
};

#endif // RENDERER_H
