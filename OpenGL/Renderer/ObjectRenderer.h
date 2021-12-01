#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include <OpenGL/Camera.h>
#include <OpenGL/Data/ObjectData.h>
#include <OpenGL/Data/RegionData.h>
#include <OpenGL/Light.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

typedef GLuint Location;
class ObjectRenderer : protected QOpenGLFunctions
{
public:
    ObjectRenderer();

    bool init();
    void render(QVector<Object *> objects, const Camera *camera, const Light *light);

protected:
    QMap<Object::Type, ObjectData *> mObjectData;

    QOpenGLShaderProgram *mShader;

    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

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
