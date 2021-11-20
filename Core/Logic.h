#ifndef LOGIC_H
#define LOGIC_H

#include <Dependencies/Eigen/Dense>

namespace Dori {
namespace Core {
class Logic
{
public:
    enum EdgeNames { BISECTOR = 0, V1 = 1, V2 = 2, V3 = 3, V4 = 4 };

    struct Frustum
    {
        Eigen::Vector3f topVertices[5]; // BISECTOR, V1, V2, V3, V4
        Eigen::Vector3f bottomVertices[5];
        float zNear;
        float zFar;
        float verticalFov;
        float horizontalFov;
        float aspectRatio;
    };

    struct Parameters
    {
        Eigen::Vector3f camera;
        Eigen::Vector3f target;
        Eigen::Vector3f lowerBoundary;
        float tiltAngle;
        Frustum frustum;
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
