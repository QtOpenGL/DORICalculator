#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Dependencies/Eigen/Core>

struct CameraParameters
{
    float height;
    float verticalFovAngle;
    float horizontalFovAngle;
    float tiltAngle;
    float fovWidth;
};

struct TargetParameters
{
    float height;
    float distance;
    float lowerBoundaryDistance;
    float lowerBoundaryHeight;
};

struct CommonParameters
{
    CameraParameters cameraParameters;
    TargetParameters targetParameters;
};

struct IntersectionPoint
{
    Eigen::Vector3f point;
    bool intersected;
};

struct OutputParameters
{
    IntersectionPoint groundIntersectionPoints[4];
    IntersectionPoint bisectorIntersectionPoint;
};

struct InputParameters
{
    CameraParameters cameraParameters;
    TargetParameters targetParameters;
};

#endif // PARAMETERS_H
