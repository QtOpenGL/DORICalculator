#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <GUI/LeftWidget.h>
#include <QObject>
#include <QPolygonF>
#include <QVector2D>

class SideViewWidget;
class TopViewWidget;
class CentralWidget;
class AxisWidget;
class OpenGLWindow;

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
        Region regions[NUMBER_OF_REGIONS];
        QPointF bisectorIntersection;
        QPointF oppositeBisectorIntersection;
    };

    struct TopViewWidgetParamaters
    {
        Camera camera;
        Target target;
        Ground ground;
        LowerBoundary lowerBoundary;
        Region regions[NUMBER_OF_REGIONS];
    };

    CentralWidget *centralWidget();

    OpenGLWindow *openGLWindow() const;

public slots:
    void onDirty();
    void onZoom(int);
    void onPan(int x, int y);
    void init();

private:
    void calculate();
    void update();

    void setMeterToPixelRatio(float newMeterToPixelRatio);
    void setOrigin(QPointF newOrigin);

    Logic &mLogic;

    Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;
    TopViewWidgetParamaters *mTopViewWidgetParameters;
    Logic::Parameters *mLeftWidgetParameters;

    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    CentralWidget *mCentralWidget;
    AxisWidget *mAxisWidget;
    LeftWidget *mLeftWidget;
    OpenGLWindow *mOpenGLWindow;

    const float mZoomStepSize;

    float mMeterToPixelRatio;
    QPointF mOrigin;
};

#endif // CONTROLLER_H
