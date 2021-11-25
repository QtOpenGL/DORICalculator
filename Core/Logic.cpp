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
    const float lowerBoundaryHeight = qMax(0.0f,
                                           qMin(mParameters->lowerBoundary.height - 0.0001f, qMin(mParameters->target.height - 0.0001f, mParameters->camera.height - 0.0001f)));
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
            if (!isnan(t) && !isinf(t) && 0 < t)
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
            if (!isnan(t) && !isinf(t) && 0 < t)
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
            if (!isnan(t) && !isinf(t) && 0 < t)
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
            if (!isnan(t) && !isinf(t) && 0 < t)
                lowerBoundary.intersections[name] = line.pointAt(t);
            else
                lowerBoundary.intersections[name] = cameraPosition + frustum.zFarNorm * frustum.edgeDirections[name];
        }

        lowerBoundary.distance = qMin(mParameters->target.distance, lowerBoundary.intersections[1].x());
        lowerBoundary.height = lowerBoundaryHeight;
    }

    Region regions[NUMBER_OF_REGIONS];
    const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));
    const Eigen::Vector3f normalAfterRotation = rotation * Eigen::Vector3f(-1, 0, 0);

    for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
        float limit = 0.5 * (sensorWidth / REGION_PPMS[i]) / tan(halfHorizontalFovRadians);

        if (limit < zNear)
            limit = zNear;

        if (limit > zFar)
            limit = zFar;

        regions[i].limit = limit;

        for (EdgeNames edgeName : {E0, E1, E2, E3}) {
            if (i == STRONG_IDENTIFICATION) {
                regions[STRONG_IDENTIFICATION].topVertices[edgeName] = frustum.topVertices[edgeName];
            } else {
                regions[i].topVertices[edgeName] = regions[i - 1].bottomVertices[edgeName]; // Top vertices is the same as previous region's bottom vertices
            }
        }

        const Eigen::Vector3f limitPointAfterRotation = Eigen::Vector3f(0, 0, mParameters->camera.height) + rotation * Eigen::Vector3f(limit, 0, 0);

        for (EdgeNames edgeName : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[edgeName]);
            Eigen::Hyperplane<float, 3> regionLimitPlaneAfterRotation(normalAfterRotation, limitPointAfterRotation);
            regions[i].bottomVertices[edgeName] = line.intersectionPoint(regionLimitPlaneAfterRotation);
        }
    }

    mParameters->frustum = frustum;
    mParameters->target = target;
    mParameters->lowerBoundary = lowerBoundary;
    mParameters->camera.tiltAngle = tiltAngle;

    std::copy(regions, regions + NUMBER_OF_REGIONS, mParameters->regions);
}

QVector<Eigen::Vector3f> Logic::findIntersection(const Region &region, const Eigen::Hyperplane<float, 3> &plane)
{
    QVector<Eigen::Vector3f> intersections3d;

    for (int i = 0; i < 4; i++)
        intersections3d << findIntersection(region.bottomVertices[i], region.bottomVertices[(i + 1) % 4], plane);

    for (int i = 0; i < 4; i++)
        intersections3d << findIntersection(region.topVertices[i], region.topVertices[(i + 1) % 4], plane);

    intersections3d << findIntersection(region.topVertices[0], region.bottomVertices[0], plane);
    intersections3d << findIntersection(region.topVertices[1], region.bottomVertices[1], plane);
    intersections3d << findIntersection(region.topVertices[2], region.bottomVertices[2], plane);
    intersections3d << findIntersection(region.topVertices[3], region.bottomVertices[3], plane);

    QVector<Eigen::Vector2f> intersections2d = projectOntoXYPlane(intersections3d);
    intersections2d = sortClockwiseOrder(intersections2d);

    QVector<Eigen::Vector3f> result;
    for (int i = 0; i < intersections2d.size(); ++i) {
        result << Eigen::Vector3f(intersections2d[i].x(), intersections2d[i].y(), 0);
    }

    return result;
}

QVector<Eigen::Vector3f> Logic::findIntersection(const Eigen::Vector3f &start, const Eigen::Vector3f &end, const Eigen::Hyperplane<float, 3> &plane)
{
    float distance = (end - start).norm();
    Eigen::Vector3f direction = (end - start).normalized();
    Eigen::ParametrizedLine<float, 3> line(start, direction);
    float t = line.intersectionParameter(plane);

    if (isnan(t) || isinf(t))
        return QVector<Eigen::Vector3f>();

    Eigen::Vector3f point = line.pointAt(t);
    if (0 < t && (start - point).norm() < distance)
        return QVector<Eigen::Vector3f>(1, point);

    return QVector<Eigen::Vector3f>();
}

QVector<Eigen::Vector2f> Logic::sortClockwiseOrder(const QVector<Eigen::Vector2f> &points)
{
    Eigen::Vector2f meanCenter = findMeanCenter(points);
    QVector<Eigen::Vector2f> newPoint = translate(points, -meanCenter);

    std::sort(newPoint.begin(), newPoint.end(), [=](const Eigen::Vector2f &p1, const Eigen::Vector2f &p2) -> bool {
        float theta1 = atan2(p1.y(), p1.x());
        if (theta1 < 0)
            theta1 += 2 * M_PI;

        float theta2 = atan2(p2.y(), p2.x());

        if (theta2 < 0)
            theta2 += 2 * M_PI;

        if (qFuzzyCompare(theta1, theta2))
            return p1.norm() < p2.norm();
        else
            return theta1 < theta2;
    });

    return translate(newPoint, meanCenter);
}

Eigen::Vector2f Logic::findMeanCenter(const QVector<Eigen::Vector2f> &points)
{
    float x = 0;
    float y = 0;
    for (const auto &point : points) {
        x += point.x();
        y += point.y();
    }

    return Eigen::Vector2f(x / points.size(), y / points.size());
}

QVector<Eigen::Vector2f> Logic::translate(const QVector<Eigen::Vector2f> &points, const Eigen::Vector2f &translation)
{
    QVector<Eigen::Vector2f> result;
    for (const auto &point : points)
        result << (point + translation);
    return result;
}

QVector<Eigen::Vector2f> Logic::projectOntoXYPlane(const QVector<Eigen::Vector3f> &points)
{
    QVector<Eigen::Vector2f> result;

    for (int i = 0; i < points.size(); ++i) {
        result << Eigen::Vector2f(points[i].x(), points[i].y());
    }

    return result;
}

Logic &Logic::getInstance()
{
    static Logic instance;
    return instance;
}
