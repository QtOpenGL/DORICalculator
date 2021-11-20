#include "Logic.h"
#include <Dependencies/Eigen/src/Geometry/AngleAxis.h>
#include <QtDebug>
#include <QtMath>

namespace Dori {
namespace Core {

Logic::Logic()
    : mGround(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), 0))
{}

Logic::Parameters Logic::calculate(const Logic::Parameters &inputParameters)
{
    const float &aspectRatio = inputParameters.frustum.aspectRatio;
    const float &horizontalFov = inputParameters.frustum.horizontalFov;
    const float &cameraHeight = inputParameters.camera.z();
    const float &targetDistance = inputParameters.target.x();
    const float &targetHeight = inputParameters.target.z();
    const float &zNear = inputParameters.frustum.zNear;
    const float &zFar = inputParameters.frustum.zFar;

    // VERTICAL FOV
    float halfHorizontalFovRadians = 0.5 * qDegreesToRadians(horizontalFov);
    float halfVerticalFovRadians = atan(tan(halfHorizontalFovRadians) / aspectRatio);
    float verticalFov = 2.0f * qRadiansToDegrees(halfVerticalFovRadians);

    // TILT ANGLE  (angle between negative x-axis and bisector of the furstum measured clockwise)
    const float tiltAngleRadians = halfVerticalFovRadians - atan2(targetHeight - cameraHeight, targetDistance);
    const float tiltAngle = qRadiansToDegrees(tiltAngleRadians);

    // Contruct frustum
    float x = 1;
    float y = x * tan(halfHorizontalFovRadians);
    float z = x * tan(halfVerticalFovRadians);

    Eigen::Vector3f edgeVectors[5];

    edgeVectors[BISECTOR] = Eigen::Vector3f(1, 0, 0);
    edgeVectors[V1] = Eigen::Vector3f(x, y, z);
    edgeVectors[V2] = Eigen::Vector3f(x, y, -z);
    edgeVectors[V3] = Eigen::Vector3f(x, -y, -z);
    edgeVectors[V4] = Eigen::Vector3f(x, -y, z);

    Frustum frustum;

    Eigen::AngleAxis<float> rotation(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));

    for (EdgeNames name : {BISECTOR, V1, V2, V3, V4}) {
        edgeVectors[name].normalize();
        Eigen::Vector3f direction = rotation * edgeVectors[name];
        frustum.topVertices[name] = inputParameters.camera + zNear * direction;

        Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(inputParameters.camera, direction);
        float t = line.intersectionParameter(mGround);

        if (t >= 0) {
            frustum.bottomVertices[name] = line.pointAt(t);
        } else {
            frustum.bottomVertices[name] = inputParameters.camera + zFar * direction;
        }
    }
    //// End of contruct frustum

    // Assign output parameters
    Parameters outputParameters = inputParameters;

    frustum.verticalFov = verticalFov;
    frustum.horizontalFov = horizontalFov;
    frustum.aspectRatio = aspectRatio;
    frustum.zNear = zNear;
    frustum.zFar = zFar;

    outputParameters.frustum = frustum;
    outputParameters.tiltAngle = tiltAngle;

    return outputParameters;
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}

} // namespace Core
} // namespace Dori
