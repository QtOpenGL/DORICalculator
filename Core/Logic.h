#ifndef LOGIC_H
#define LOGIC_H

#include <Dependencies/Eigen/Dense>

namespace Dori {
namespace Core {
class Logic
{
public:
    enum IntersectionType { EMPTY = 0, NON_EMPTY = 1 };

    struct Edge
    {
        Eigen::Vector3f direction;
        Eigen::Vector3f origin;
        Eigen::ParametrizedLine<float, 3> line;
        IntersectionType intersectionType;
        Eigen::Vector3f intersectionPoint;
    };

    struct Frustum
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        Edge edges[4];
        Edge bisector;
        float zNear;
        float zFar;
    };

    struct Parameters
    {
        float cameraHeight;
        float targetHeight;
        float targetDistance;
        float lowerBoundaryHeight;
        float mLowerBoundaryDistance;
        float verticalFov;
        float horizontalFov;
        float aspectRatio;
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
