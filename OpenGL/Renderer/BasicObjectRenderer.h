#ifndef BASICOBJECTRENDERER_H
#define BASICOBJECTRENDERER_H

#include "../Camera.h"
#include "../Light.h"
#include "../Node/BasicObject.h"
#include "../Node/CuboidData.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>

class BasicObjectRenderer : protected QOpenGLFunctions
{
public:
    BasicObjectRenderer();

    bool init();
    void render(QVector<BasicObject *> objects, const Camera *camera, const Light *light);

private:
    QMap<BasicObject::Type, BasicObjectData *> mObjectData;

    QOpenGLShaderProgram *mShader;

    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    int mModelMatrixLocation;
    int mViewMatrixLocation;
    int mProjectionMatrixLocation;
    int mObjectColorLocation;
    int mAmbientStrengthLocation;
    int mCameraPositionLocation;
    int mLightPositionLocation;
    int mLightColorLocation;
    int mLightPowerLocation;

    bool mInit;
};

#endif // BASICOBJECTRENDERER_H
