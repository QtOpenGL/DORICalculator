#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGL/Camera.h>
#include <OpenGL/Data/ModelData.h>
#include <OpenGL/Data/RegionData.h>
#include <OpenGL/Light.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Renderer : protected QOpenGLFunctions
{
public:
    Renderer();

    bool init();
    void render(QVector<Object *> objects, const Camera *camera, const Light *light);

protected:
    QMap<Model::Type, ModelData *> mModelData;

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
    int mObjectColorLocation;

    bool mInit;
};

#endif // RENDERER_H
