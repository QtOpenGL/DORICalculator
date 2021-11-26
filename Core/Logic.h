#ifndef LOGIC_H
#define LOGIC_H

#include "Enums.h"
#include <Dependencies/Eigen/Dense>
#include <QVector>

class Logic
{
public:
    struct Region
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        float limit;
    };

    struct Frustum
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        Eigen::Vector3f bisectorRay;
        Eigen::Vector3f oppositeBisectorRay;
        Eigen::Vector3f edgeDirections[4];
        float horizontalFov;
        float verticalFov;
        float zNear;
        float zFar;
        float zNearNorm;
        float zFarNorm;
    };

    struct Sensor
    {
        float width;
        float height;
        float aspectRatio;
    };

    struct Camera
    {
        Sensor sensor;
        float height;
        float tiltAngle;
    };

    struct Entity
    {
        float height;
        float distance;
        Eigen::Vector3f intersections[4];
        float fovWidth;
    };

    struct Parameters
    {
        Frustum frustum;
        Camera camera;
        Entity target;
        Entity lowerBoundary;
        Region regions[NUMBER_OF_REGIONS];
    };

public:
    static Logic &getInstance();
    void calculate();
    QVector<Eigen::Vector3f> findIntersection(const Region &region, const Eigen::Hyperplane<float, 3> &plane);

    float calculateHorizontalFovForGivenFovWidth(float fovWidth);
    float validateTargetDistance(float newTargetDistance);
    float validateTargetHeight(float newTargetHeight);
    float validateCameraHeight(float newCameraHeight);

    Parameters *parameters() const;
    void setParameters(Parameters *newParameters);

private:
    QVector<Eigen::Vector3f> findIntersection(const Eigen::Vector3f &start, const Eigen::Vector3f &end, const Eigen::Hyperplane<float, 3> &plane);
    QVector<Eigen::Vector2f> sortClockwiseOrder(const QVector<Eigen::Vector2f> &points);
    Eigen::Vector2f findMeanCenter(const QVector<Eigen::Vector2f> &points);
    QVector<Eigen::Vector2f> translate(const QVector<Eigen::Vector2f> &points, const Eigen::Vector2f &translation);
    QVector<Eigen::Vector2f> projectOntoXYPlane(const QVector<Eigen::Vector3f> &points);

private:
    Logic();

    const Eigen::Hyperplane<float, 3> mGround;
    Parameters *mParameters;

    inline float f(double x, double c, double k, float aspectRatio) { return k * cos(x - c) + k * tan(x) * sin(x - c) - aspectRatio * tan(x); }
};

#endif // LOGIC_H
