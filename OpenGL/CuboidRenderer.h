#ifndef CUBOIDRENDERER_H
#define CUBOIDRENDERER_H

#include "Camera.h"
#include "Cuboid.h"
#include "Light.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class CuboidRenderer : protected QOpenGLFunctions
{
public:
    CuboidRenderer();

    bool initialize();
    void render(QVector<Object *> objects, const Camera &camera, const Light &light);

private:
    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    bool mInitialized;

    QOpenGLShaderProgram *mShader = nullptr;

    int mModelMatrixLocation = 0;
    int mViewMatrixLocation = 0;
    int mProjectionMatrixLocation = 0;
    int mObjectColorLocation = 0;
    int mLightColorLocation = 0;
    int mLightPositionLocation = 0;
    int mAmbientStrengthLocation;

    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;

    static const float VERTEX_DATA[216];
};

#endif // CUBOIDRENDERER_H
