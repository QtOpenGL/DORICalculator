#include "Logic.h"

#include <Core/Enums.h>
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
    float aspectRatio = inputParameters.frustum.aspectRatio;
    float horizontalFov = inputParameters.frustum.horizontalFov;
    float cameraHeight = inputParameters.camera.height;
    float targetDistance = inputParameters.target.distance;
    float targetHeight = inputParameters.target.height;

    float zNear = inputParameters.frustum.zNear;
    float zFar = inputParameters.frustum.zFar;

    float lowerBoundaryHeight = inputParameters.lowerBoundary.height;

    // Validation
    if (lowerBoundaryHeight <= 0)
        lowerBoundaryHeight = 0;

    if (lowerBoundaryHeight >= targetHeight)
        lowerBoundaryHeight = targetHeight;

    // Vertical Fov
    float halfHorizontalFovRadians = 0.5 * qDegreesToRadians(horizontalFov);
    float halfVerticalFovRadians = atan(tan(halfHorizontalFovRadians) / aspectRatio);
    float verticalFov = 2.0f * qRadiansToDegrees(halfVerticalFovRadians);

    // Tilt angle (angle between negative x-axis and bisector of the furstum measured clockwise)
    const float tiltAngleRadians = halfVerticalFovRadians - atan2(targetHeight - cameraHeight, targetDistance);
    const float tiltAngle = qRadiansToDegrees(tiltAngleRadians);

    // Contruct frustum
    float x = 1;
    float y = x * tan(halfHorizontalFovRadians);
    float z = x * tan(halfVerticalFovRadians);

    Eigen::Vector3f edgeVectors[6];

    edgeVectors[OPPOSITE_BISECTOR] = Eigen::Vector3f(-1, 0, 0);
    edgeVectors[BISECTOR] = Eigen::Vector3f(1, 0, 0);
    edgeVectors[V1] = Eigen::Vector3f(x, y, z);
    edgeVectors[V2] = Eigen::Vector3f(x, y, -z);
    edgeVectors[V3] = Eigen::Vector3f(x, -y, -z);
    edgeVectors[V4] = Eigen::Vector3f(x, -y, z);

    Frustum frustum;
    Eigen::Vector3f cameraPosition = Eigen::Vector3f(0, 0, cameraHeight);

    Eigen::AngleAxis<float> rotation(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));

    for (EdgeNames name : {OPPOSITE_BISECTOR, BISECTOR, V1, V2, V3, V4}) {
        edgeVectors[name].normalize();
        edgeVectors[name] = rotation * edgeVectors[name];
        frustum.topVertices[name] = cameraPosition + zNear * edgeVectors[name];

        Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
        float t = line.intersectionParameter(mGround);

        if (t >= 0) {
            frustum.bottomVertices[name] = line.pointAt(t);
        } else {
            frustum.bottomVertices[name] = cameraPosition + zFar * edgeVectors[name];
        }
    }

    // Target and lower boundary intersections
    Target target;
    LowerBoundary lowerBoundary;

    for (EdgeNames name : {V1, V2, V3, V4}) {
        // Target
        {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -targetHeight)); // above z axis means negative offset

            if (t >= 0) {
                target.intersections[name - V1] = line.pointAt(t);
            } else {
                target.intersections[name - V1] = cameraPosition + zFar * edgeVectors[name];
            }
        }

        // Lower Boundary
        {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -lowerBoundaryHeight)); // above z axis means negative offset

            if (t >= 0) {
                lowerBoundary.intersections[name - V1] = line.pointAt(t);
            } else {
                lowerBoundary.intersections[name - V1] = cameraPosition + zFar * edgeVectors[name];
            }
        }
    }

    // Assign output parameters
    Parameters outputParameters;

    frustum.verticalFov = verticalFov;
    frustum.horizontalFov = horizontalFov;
    frustum.aspectRatio = aspectRatio;
    frustum.zNear = zNear;
    frustum.zFar = zFar;
    outputParameters.frustum = frustum;

    outputParameters.camera.height = cameraHeight;
    outputParameters.camera.tiltAngle = tiltAngle;

    outputParameters.target = target;
    outputParameters.target.distance = targetDistance;
    outputParameters.target.height = targetHeight;

    outputParameters.lowerBoundary = lowerBoundary;
    outputParameters.lowerBoundary.distance = lowerBoundary.intersections[1].x();
    outputParameters.lowerBoundary.height = lowerBoundaryHeight;

    return outputParameters;
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}

} // namespace Core
} // namespace Dori
