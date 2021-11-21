#ifndef LOGIC_H
#define LOGIC_H

#include <Dependencies/Eigen/Dense>

namespace Dori {
namespace Core {
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
        float aspectRatio;
    };

    struct Camera
    {
        float height;
        float tiltAngle;
    };

    struct Target
    {
        float height;
        float distance;
        Eigen::Vector3f intersections[4];
    };

    struct LowerBoundary : Target
    {};

    struct Parameters
    {
        Frustum frustum;
        Camera camera;
        Target target;
        LowerBoundary lowerBoundary;
    };

private:
    Logic();

public:
    static Logic &getInstance();
    Parameters calculate(const Parameters &newParameters);

private:
    const Eigen::Hyperplane<float, 3> mGround;
};
} // namespace Core
} // namespace Dori

#endif // LOGIC_H
