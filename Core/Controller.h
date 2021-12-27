#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include "Typedefs.h"
#include <Dependencies/Eigen/src/Geometry/Hyperplane.h>

#include <QObject>
#include <QPolygonF>
#include <QVector2D>

class CentralWidget;
class CameraWidget;
class CursorPositionWidget;
class TargetWidget;
class LowerBoundaryWidget;
class OpenGLWidget;
class SideViewWidget;
class TopViewWidget;
class AxisWidget;
class RegionInfoWidget;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

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

    struct OpenGLWidgetRegion
    {
        QVector<QVector3D> vertices;
        bool intersectsGround;
    };

    struct OpenGLWidgetParameters
    {
        OpenGLWidgetRegion regions[7];
        QVector<QVector3D> frustumEdgeVertices;
        float cameraHeight;
        float tiltAngle;
    };

    CentralWidget *centralWidget() const;

    OpenGLWidget *openGLWidget() const;

public slots:
    void onDirty();
    void onZoom(int);
    void onPan(int x, int y);

private:
    void update();
    void updateSideViewWidgetParameters();
    void updateTopViewWidgetParameters();
    void updateOpenGLWindowParameters();

    bool intersectsGround(const OpenGLWidgetRegion &region);
    QVector<QVector3D> convertToOpenGLConvention(const QVector<Eigen::Vector3f> &vectors);
    QVector<QVector3D> createFrustumEdgeVerticesForOpenGLWindow(const Logic::Frustum &frustum);
    QVector<QVector3D> createVerticesForOpenGLWindow(const Logic::Region &region);
    QVector<QVector3D> createNormalsForOpenGLWindow(const QVector<QVector3D> &vertices);

    void setMeterToPixelRatio(float newMeterToPixelRatio);
    void setOrigin(QPointF newOrigin);

    Logic *mLogic;

    Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;
    TopViewWidgetParameters *mTopViewWidgetParameters;
    OpenGLWidgetParameters *mOpenGLWidgetParameters;

    CentralWidget *mCentralWidget;
    CameraWidget *mCameraWidget;
    TargetWidget *mTargetWidget;
    LowerBoundaryWidget *mLowerBoundaryWidget;
    CursorPositionWidget *mCursorPositionWidget;
    RegionInfoWidget *mRegionInfoWidget;

    SideViewWidget *mSideViewWidget;
    AxisWidget *mAxisWidget;
    TopViewWidget *mTopViewWidget;

    OpenGLWidget *mOpenGLWidget;

    const float mZoomStepSize;

    float mMeterToPixelRatio;
    QPointF mOrigin;
    Eigen::Hyperplane<float, 3> mGround;
};

#endif // CONTROLLER_H
