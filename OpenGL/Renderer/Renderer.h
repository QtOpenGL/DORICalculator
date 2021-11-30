#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGL/Camera.h>
#include <OpenGL/Light.h>
#include <OpenGL/Node/Node.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Renderer : protected QOpenGLFunctions
{
public:
    Renderer(QString vertexShaderPath, QString fragmentShaderPath);

    virtual bool init();
    virtual void render(QVector<Node *> nodes, const Camera *camera, const Light *light) = 0;

protected:
    QOpenGLShaderProgram *mShader;

    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    int mModelMatrixLocation;
    int mViewMatrixLocation;
    int mProjectionMatrixLocation;
    int mAmbientStrengthLocation;
    int mCameraPositionLocation;
    int mLightPositionLocation;
    int mLightColorLocation;
    int mLightPowerLocation;
    int mColorLocation;
};

#endif // RENDERER_H
