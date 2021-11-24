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

    Parameters *parameters() const;
    void setParameters(Parameters *newParameters);

private:
    QVector<Eigen::Vector3f> findIntersection(const Eigen::Vector3f &start, const Eigen::Vector3f &end, const Eigen::Hyperplane<float, 3> &plane);

private:
    Logic();

    const Eigen::Hyperplane<float, 3> mGround;
    Parameters *mParameters;
};

#endif // LOGIC_H
