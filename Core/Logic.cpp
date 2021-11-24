#include "Logic.h"

#include <Core/Constants.h>
#include <Core/Enums.h>
#include <Dependencies/Eigen/src/Geometry/AngleAxis.h>

#include <QtDebug>
#include <QtMath>

Logic::Logic()
    : mGround(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), 0))
{}

Logic::Parameters *Logic::parameters() const
{
    return mParameters;
}

void Logic::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}

void Logic::calculate()
{
    const float lowerBoundaryHeight = qMax(0.0f, qMin(mParameters->lowerBoundary.height, qMin(mParameters->target.height, mParameters->camera.height)));
    const Eigen::Vector3f cameraPosition = Eigen::Vector3f(0, 0, mParameters->camera.height);

    // Vertical Fov
    const float halfHorizontalFovRadians = 0.5 * qDegreesToRadians(mParameters->frustum.horizontalFov);
    const float halfVerticalFovRadians = atan(tan(halfHorizontalFovRadians) / mParameters->camera.sensor.aspectRatio);

    // Tilt angle (angle between negative x-axis and bisector of the furstum measured clockwise)
    const float tiltAngleRadians = halfVerticalFovRadians - atan2(mParameters->target.height - mParameters->camera.height, mParameters->target.distance);
    const float tiltAngle = qRadiansToDegrees(tiltAngleRadians);

    const float zNear = mParameters->frustum.zNear;
    const float zFar = mParameters->frustum.zFar;
    const float sensorWidth = mParameters->camera.sensor.width;

    // Construct frustum
    Frustum frustum;
    {
        const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));
        const float zNearNorm = Eigen::Vector3f(zNear, zNear * tan(halfHorizontalFovRadians), zNear * tan(halfVerticalFovRadians)).norm();
        const float zFarNorm = Eigen::Vector3f(zFar, zFar * tan(halfHorizontalFovRadians), zFar * tan(halfVerticalFovRadians)).norm();

        const float x = 1;
        const float y = x * tan(halfHorizontalFovRadians);
        const float z = x * tan(halfVerticalFovRadians);

        frustum.edgeDirections[E0] = Eigen::Vector3f(x, y, z);
        frustum.edgeDirections[E1] = Eigen::Vector3f(x, y, -z);
        frustum.edgeDirections[E2] = Eigen::Vector3f(x, -y, -z);
        frustum.edgeDirections[E3] = Eigen::Vector3f(x, -y, z);

        for (EdgeNames name : {E0, E1, E2, E3}) {
            frustum.edgeDirections[name].normalize();
            frustum.edgeDirections[name] = rotation * frustum.edgeDirections[name];
            frustum.topVertices[name] = cameraPosition + zNearNorm * frustum.edgeDirections[name];
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(mGround);
            if (t >= 0)
                frustum.bottomVertices[name] = line.pointAt(t);
            else
                frustum.bottomVertices[name] = cameraPosition + zFarNorm * frustum.edgeDirections[name];
        }

        Eigen::Vector3f bisectors[2];
        bisectors[0] = Eigen::Vector3f(1, 0, 0);
        bisectors[1] = Eigen::Vector3f(-1, 0, 0);

        for (int i = 0; i < 2; ++i) {
            bisectors[i].normalize();
            bisectors[i] = rotation * bisectors[i];

            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, bisectors[i]);
            float t = line.intersectionParameter(mGround);
            if (t >= 0)
                bisectors[i] = line.pointAt(t);
            else
                bisectors[i] = cameraPosition + zFar * bisectors[i];
        }

        frustum.bisectorRay = bisectors[0];
        frustum.oppositeBisectorRay = bisectors[1];
        frustum.zNear = zNear;
        frustum.zFar = zFar;
        frustum.zNearNorm = zNearNorm;
        frustum.zFarNorm = zFarNorm;
        frustum.horizontalFov = 2 * qRadiansToDegrees(halfHorizontalFovRadians);
        frustum.verticalFov = 2 * qRadiansToDegrees(halfVerticalFovRadians);
    }

    //Find target intersections
    Entity target;
    {
        for (EdgeNames name : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -mParameters->target.height)); // above z axis means negative offset
            if (t >= 0)
                target.intersections[name] = line.pointAt(t);
            else
                target.intersections[name] = cameraPosition + frustum.zFarNorm * frustum.edgeDirections[name];
        }

        target.distance = mParameters->target.distance;
        target.height = mParameters->target.height;
    }

    // Find lower boundary intersections
    Entity lowerBoundary;
    {
        for (EdgeNames name : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -lowerBoundaryHeight)); // above z axis means negative offset
            if (t >= 0)
                lowerBoundary.intersections[name] = line.pointAt(t);
            else
                lowerBoundary.intersections[name] = cameraPosition + frustum.zFarNorm * frustum.edgeDirections[name];
        }

        lowerBoundary.distance = qMin(mParameters->target.distance, lowerBoundary.intersections[1].x());
        lowerBoundary.height = lowerBoundaryHeight;
    }

    Region regions[6];
    const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));
    const Eigen::Vector3f normalAfterRotation = rotation * Eigen::Vector3f(-1, 0, 0);

    for (RegionNames regionName : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING}) {
        float limit = 0.5 * (sensorWidth / REGION_PPMS[regionName]) / tan(halfHorizontalFovRadians);

        if (limit < zNear || limit > zFar) {
            regions[regionName].visible = false;

        } else {
            regions[regionName].limit = limit;
            regions[regionName].visible = true;

            // STRONG_IDENTIFICATION

            for (EdgeNames edgeName : {E0, E1, E2, E3}) {
                if (regionName == STRONG_IDENTIFICATION) {
                    regions[STRONG_IDENTIFICATION].topVertices[edgeName] = frustum.topVertices[edgeName]; // Top vertices is the same as previous region's bottom vertices
                } else {
                    regions[regionName].topVertices[edgeName] = regions[regionName - 1].bottomVertices[edgeName]; // Top vertices is the same as previous region's bottom vertices
                }
            }

            const Eigen::Vector3f limitPointAfterRotation = rotation * Eigen::Vector3f(limit, 0, 0);

            for (EdgeNames edgeName : {E0, E1, E2, E3}) {
                Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[edgeName]);
                Eigen::Hyperplane<float, 3> regionLimitPlaneAfterRotation(normalAfterRotation, limitPointAfterRotation);
                regions[regionName].bottomVertices[edgeName] = line.intersectionPoint(regionLimitPlaneAfterRotation);
            }
        }
    }

    mParameters->frustum = frustum;
    mParameters->target = target;
    mParameters->lowerBoundary = lowerBoundary;
    mParameters->camera.tiltAngle = tiltAngle;

    std::copy(regions, regions + 6, mParameters->regions);
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}
