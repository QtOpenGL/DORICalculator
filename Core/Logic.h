#ifndef LOGIC_H
#define LOGIC_H

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
        bool visible;
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
        Region regions[6];
    };

public:
    static Logic &getInstance();
    void calculate();

    Parameters *outputParameters() const;
    void setOutputParameters(Parameters *newOutputParameters);

    Parameters *inputParameters() const;
    void setInputParameters(Parameters *newInputParameters);

private:
    Logic();

    const Eigen::Hyperplane<float, 3> mGround;
    Parameters *mInputParameters;
    Parameters *mOutputParameters;
};

#endif // LOGIC_H
