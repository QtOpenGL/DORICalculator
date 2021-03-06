#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include <Core/Typedefs.h>
#include <OpenGL/Camera.h>
#include <OpenGL/Data/ObjectData.h>
#include <OpenGL/Light.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class ObjectRenderer : protected QOpenGLFunctions
{
public:
    ObjectRenderer();

    bool init();
    void render(QVector<Object *> objects, const Camera *camera, const Light *light);

protected:
    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    QMap<Object::Type, ObjectData *> mObjectData;

    QOpenGLShaderProgram *mShader;

    Location mModelMatrix;
    Location mViewMatrix;
    Location mProjectionMatrix;
    Location mAmbientStrength;
    Location mCameraPosition;
    Location mLightPosition;
    Location mLightColor;
    Location mLightPower;
    Location mObjectColor;

    bool mInit;
};

#endif // OBJECTRENDERER_H
