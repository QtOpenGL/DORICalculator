#ifndef REGIONRENDERER_H
#define REGIONRENDERER_H

#include <Core/Typedefs.h>
#include <OpenGL/Camera.h>
#include <OpenGL/Data/RegionData.h>
#include <OpenGL/Light.h>

#include <QOpenGLShaderProgram>

class RegionRenderer : protected QOpenGLFunctions
{
public:
    RegionRenderer();

    bool init();
    void render(RegionData regionData[7], const Camera *camera, const Light *light);

protected:
    const QString mVertexShaderPath;
    const QString mFragmentShaderPath;

    QOpenGLShaderProgram *mShader;

    Location mViewMatrix;
    Location mProjectionMatrix;
    Location mAmbientStrength;
    Location mCameraPosition;
    Location mLightPosition;
    Location mLightColor;
    Location mLightPower;
    Location mRegionColor;

    bool mInit;
};

#endif // REGIONRENDERER_H
