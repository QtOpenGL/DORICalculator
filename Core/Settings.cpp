#include "Settings.h"

namespace Dori {
namespace Core {

Settings::Settings()
{
    mDefaultParameters.cameraHeight = 100;
    mDefaultParameters.targetHeight = 100;
    mDefaultParameters.targetDistance = 200;
    mDefaultParameters.lowerBoundaryHeight = 0;
    mDefaultParameters.horizontalFov = 60;
    mDefaultParameters.aspectRatio = 16.0f / 9.0f;
    mDefaultParameters.frustum.zNear = 0;
    mDefaultParameters.frustum.zFar = 1000;
}

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

const Logic::Parameters Settings::defaultParameters()
{
    return mDefaultParameters;
}

} // namespace Core
} // namespace Dori
