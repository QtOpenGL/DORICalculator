#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Dependencies/Eigen/Core>
#include <QPointF>
#include <QPolygonF>
#include <QVector3D>
#include <QVector>

namespace Logic {
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
    Region regions[7];
};

} // namespace Logic

namespace Controller {

struct Camera
{
    float height;
    float tiltAngle;
    float horizontalFov;
    float verticalFov;
    QPointF position;
};

struct Target
{
    float fovWidth;
    float height;
    float distance;
    QPointF position;
    QPointF intersections[4];
};

struct LowerBoundary
{
    float height;
    float distance;
    QPointF position;
    QPointF intersections[4];
};

struct Ground
{
    QPointF intersections[4];
};

struct Region
{
    QPolygonF region;
    bool visible;
};

struct SideViewWidgetParameters
{
    Camera camera;
    Target target;
    Ground ground;
    LowerBoundary lowerBoundary;
    Region regions[7];
    QPointF bisectorIntersection;
    QPointF oppositeBisectorIntersection;
};

struct TopViewWidgetParameters
{
    Camera camera;
    Target target;
    Ground ground;
    LowerBoundary lowerBoundary;
    Region regions[7];
};

struct OpenGLWindowRegion
{
    QVector3D edgeVertices[8];
    QVector3D vertices[8];
    bool intersectsGround;
};

typedef OpenGLWindowRegion OpenGLWindowFrustum;

struct OpenGLWindowParameters
{
    OpenGLWindowRegion regions[7];
    OpenGLWindowFrustum frustum;
};

} // namespace Controller

#endif // PARAMETERS_H
