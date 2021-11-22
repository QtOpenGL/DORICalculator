#include "Logic.h"

#include <Core/Constants.h>
#include <Core/Enums.h>
#include <Dependencies/Eigen/src/Geometry/AngleAxis.h>

#include <QtDebug>
#include <QtMath>

Logic::Logic()
    : mGround(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), 0))
{}

Logic::Parameters Logic::calculate(const Logic::Parameters &inputParameters)
{
    const float aspectRatio = inputParameters.camera.sensor.aspectRatio;
    const float horizontalFov = inputParameters.frustum.horizontalFov;
    const float cameraHeight = inputParameters.camera.height;
    const float targetDistance = inputParameters.target.distance;
    const float targetHeight = inputParameters.target.height;

    const float zNear = inputParameters.frustum.zNear;
    const float zFar = inputParameters.frustum.zFar;

    const float sensorWidth = inputParameters.camera.sensor.width;

    float lowerBoundaryHeight = qMax(0.0f, qMin(inputParameters.lowerBoundary.height, qMin(targetHeight, cameraHeight)));

    // Vertical Fov
    const float halfHorizontalFovRadians = 0.5 * qDegreesToRadians(horizontalFov);
    const float halfVerticalFovRadians = atan(tan(halfHorizontalFovRadians) / aspectRatio);
    const float verticalFov = 2.0f * qRadiansToDegrees(halfVerticalFovRadians);

    // Tilt angle (angle between negative x-axis and bisector of the furstum measured clockwise)
    const float tiltAngleRadians = halfVerticalFovRadians - atan2(targetHeight - cameraHeight, targetDistance);
    const float tiltAngle = qRadiansToDegrees(tiltAngleRadians);

    // Contruct frustum
    const float x = 1;
    const float y = x * tan(halfHorizontalFovRadians);
    const float z = x * tan(halfVerticalFovRadians);

    Eigen::Vector3f edgeVectors[6];
    edgeVectors[OPPOSITE_BISECTOR] = Eigen::Vector3f(-1, 0, 0);
    edgeVectors[BISECTOR] = Eigen::Vector3f(1, 0, 0);
    edgeVectors[V1] = Eigen::Vector3f(x, y, z);
    edgeVectors[V2] = Eigen::Vector3f(x, y, -z);
    edgeVectors[V3] = Eigen::Vector3f(x, -y, -z);
    edgeVectors[V4] = Eigen::Vector3f(x, -y, z);

    Eigen::Vector3f cameraPosition = Eigen::Vector3f(0, 0, cameraHeight);
    Frustum frustum;

    {
        Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));

        for (EdgeNames name : {OPPOSITE_BISECTOR, BISECTOR, V1, V2, V3, V4}) {
            edgeVectors[name].normalize();
            edgeVectors[name] = rotation * edgeVectors[name];
            frustum.topVertices[name] = cameraPosition + zNear * edgeVectors[name];

            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
            float t = line.intersectionParameter(mGround);

            if (t >= 0)
                frustum.bottomVertices[name] = line.pointAt(t);
            else
                frustum.bottomVertices[name] = cameraPosition + zFar * edgeVectors[name];
        }
    }

    // Target and lower boundary intersections

    Target target;
    LowerBoundary lowerBoundary;
    {
        for (EdgeNames name : {V1, V2, V3, V4}) {
            // Target
            {
                Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
                float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -targetHeight)); // above z axis means negative offset

                if (t >= 0)
                    target.intersections[name - V1] = line.pointAt(t);
                else
                    target.intersections[name - V1] = cameraPosition + zFar * edgeVectors[name];
            }

            // Lower Boundary
            {
                Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, edgeVectors[name]);
                float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -lowerBoundaryHeight)); // above z axis means negative offset

                if (t >= 0)
                    lowerBoundary.intersections[name - V1] = line.pointAt(t);
                else
                    lowerBoundary.intersections[name - V1] = cameraPosition + zFar * edgeVectors[name];
            }
        }
    }

    Zone zones[7];
    // Zones
    {
        // STRONG_IDENTIFICATION
        {
            Eigen::Vector3f edgeVectors[4];
            float limit = 0.5 * (sensorWidth / ZONE_PPMS[STRONG_IDENTIFICATION]) / tan(halfHorizontalFovRadians);
            constructEdgeVectorsForZones(edgeVectors, limit, tiltAngleRadians, halfHorizontalFovRadians, halfVerticalFovRadians);

            if (limit < zNear || limit > zFar) {
                zones[STRONG_IDENTIFICATION].insideFrustum = false;
            } else {
                zones[STRONG_IDENTIFICATION].insideFrustum = true;
                for (int i = 0; i < 4; ++i) {
                    zones[STRONG_IDENTIFICATION].topVertices[i] = frustum.topVertices[i];
                    zones[STRONG_IDENTIFICATION].bottomVertices[i] = cameraPosition + edgeVectors[i];
                }
            }
        }

        // IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING
        for (ZoneNames zone : {IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING}) {
            float limit = 0.5 * (sensorWidth / ZONE_PPMS[zone]) / tan(halfHorizontalFovRadians);

            if (limit < zNear || limit > zFar) {
                zones[zone].insideFrustum = false;
                continue;
            }

            Eigen::Vector3f edgeVectors[4];
            constructEdgeVectorsForZones(edgeVectors, limit, tiltAngleRadians, halfHorizontalFovRadians, halfVerticalFovRadians);

            zones[zone].insideFrustum = true;

            for (int i = 0; i < 4; ++i) {
                zones[zone].topVertices[i] = zones[zone - 1].bottomVertices[i];
                zones[zone].bottomVertices[i] = cameraPosition + edgeVectors[i];
            }
        }

        Eigen::Vector3f edgeVectors[4];
        constructEdgeVectorsForZones(edgeVectors, zFar, tiltAngleRadians, halfHorizontalFovRadians, halfVerticalFovRadians);

        // DEAD_ZONE
        for (int i = 0; i < 4; ++i) {
            zones[DEAD_ZONE].insideFrustum = true;
            zones[DEAD_ZONE].topVertices[i] = zones[MONITORING].bottomVertices[i];
            zones[DEAD_ZONE].bottomVertices[i] = cameraPosition + edgeVectors[i]; // V1, V2, V3, V4
        }
    }

    // Assign output parameters
    Parameters outputParameters;

    frustum.verticalFov = verticalFov;
    frustum.horizontalFov = horizontalFov;
    frustum.zNear = zNear;
    frustum.zFar = zFar;
    outputParameters.frustum = frustum;

    outputParameters.camera.sensor = inputParameters.camera.sensor; /////
    outputParameters.camera.height = cameraHeight;
    outputParameters.camera.tiltAngle = tiltAngle;

    outputParameters.target = target;
    outputParameters.target.distance = targetDistance;
    outputParameters.target.height = targetHeight;

    outputParameters.lowerBoundary = lowerBoundary;
    outputParameters.lowerBoundary.distance = qMin(targetDistance, lowerBoundary.intersections[1].x());
    outputParameters.lowerBoundary.height = lowerBoundaryHeight;

    for (ZoneNames zone : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING, DEAD_ZONE}) {
        outputParameters.zones[zone] = zones[zone];
    }

    return outputParameters;
}

void Logic::constructEdgeVectorsForZones(Eigen::Vector3f *edgeVectors, float limit, float tiltAngleRadians, float halfHorizontalFovRadians, float halfVerticalFovRadians)
{
    const float x = limit;
    const float y = limit * tan(halfHorizontalFovRadians);
    const float z = limit * tan(halfVerticalFovRadians);

    edgeVectors[0] = Eigen::Vector3f(x, y, z);
    edgeVectors[1] = Eigen::Vector3f(x, y, -z);
    edgeVectors[2] = Eigen::Vector3f(x, -y, -z);
    edgeVectors[3] = Eigen::Vector3f(x, -y, z);

    Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));

    for (int i = 0; i < 4; i++) {
        float norm = edgeVectors[i].norm();
        edgeVectors[i].normalize();
        edgeVectors[i] = rotation * edgeVectors[i];
        edgeVectors[i] = norm * edgeVectors[i];
    }
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}
