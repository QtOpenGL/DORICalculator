#include "Logic.h"
#include "Settings.h"
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
    const float aspectRatio = inputParameters.aspectRatio;
    const float horizontalFov = inputParameters.horizontalFov;
    const float cameraHeight = inputParameters.cameraHeight;
    const float targetDistance = inputParameters.targetDistance;
    const float targetHeight = inputParameters.targetHeight;

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

    Edge edges[4];

    for (int i = 0; i < 4; ++i)
        edges[i].origin = Eigen::Vector3f(0, 0, cameraHeight);

    edges[0].direction = Eigen::Vector3f(x, y, z);
    edges[1].direction = Eigen::Vector3f(x, y, -z);
    edges[2].direction = Eigen::Vector3f(x, -y, -z);
    edges[3].direction = Eigen::Vector3f(x, -y, z);

    Eigen::AngleAxis<float> rotation(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));

    for (int i = 0; i < 4; ++i)
        edges[i].direction = rotation * edges[i].direction;

    for (int i = 0; i < 4; ++i)
        edges[i].direction.normalize();

    for (int i = 0; i < 4; ++i)
        edges[i].line = Eigen::ParametrizedLine<float, 3>(edges[i].origin, edges[i].direction);

    // Find intersections with ground
    for (int i = 0; i < 4; ++i) {
        float t = edges[i].line.intersectionParameter(mGround);

        if (t >= 0) {
            edges[i].intersectionType = IntersectionType::NON_EMPTY;
            edges[i].intersectionPoint = edges[i].line.pointAt(t);
        } else {
            edges[i].intersectionType = IntersectionType::EMPTY;
        }

        QMessageLogger(__FILE__, __LINE__, __FUNCTION__).debug() << QString("t = %1").arg(t, 5, 'f', 2, ' ');
    }

    // Assign output parameters
    Parameters outputParameters = inputParameters;

    for (int i = 0; i < 4; ++i)
        outputParameters.frustum.edges[i] = edges[i];

    outputParameters.tiltAngle = tiltAngle;
    outputParameters.verticalFov = verticalFov;

    return outputParameters;
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}

} // namespace Core
} // namespace Dori
