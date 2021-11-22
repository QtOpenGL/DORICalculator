#ifndef LOGIC_H
#define LOGIC_H

#include <Dependencies/Eigen/Dense>

class Logic
{
public:
    struct Frustum
    {
        Eigen::Vector3f topVertices[6]; // OPPOSITE_BISECTOR, BISECTOR, V1, V2, V3, V4
        Eigen::Vector3f bottomVertices[6];
        float zNear;
        float zFar;
        float verticalFov;
        float horizontalFov;
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

    struct Target
    {
        float height;
        float distance;
        Eigen::Vector3f intersections[8];
    };

    struct LowerBoundary : Target
    {};

    struct Zone
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        bool visible;
    };

    struct Parameters
    {
        Frustum frustum;
        Camera camera;
        Target target;
        LowerBoundary lowerBoundary;
        Zone zones[7];
    };

private:
    Logic();

public:
    static Logic &getInstance();
    Parameters calculate(const Parameters &newParameters);
    void constructEdgeVectorsForZones(Eigen::Vector3f *edgeVectors, float limit, float tiltAngleRadians, float halfHorizontalFovRadians, float halfVerticalFovRadians);

private:
    const Eigen::Hyperplane<float, 3> mGround;
};

#endif // LOGIC_H
