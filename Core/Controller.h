#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <GUI/Widgets/LeftWidget.h>
#include <QObject>
#include <QPolygonF>
#include <QVector2D>

class SideViewWidget;
class TopViewWidget;
class CentralWidget;
class AxisWidget;

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
        float height;
        float distance;
        float fovWidth;
        QPointF position;
    };

    struct LowerBoundary : Target
    {};

    struct Region
    {
        QPolygonF region;
        bool visible;
    };

    struct SideViewWidgetParameters
    {
        Camera camera;
        Target target;
        LowerBoundary lowerBoundary;
        QPointF points[4];
        Region regions[NUMBER_OF_REGIONS];
    };

    struct TopViewWidgetParamaters
    {
        Target target;
        QPointF groundIntersections[4];
        QPointF targetIntersections[4];
        QPointF lowerBoundaryIntersections[4];
        Region regions[NUMBER_OF_REGIONS];
    };

    CentralWidget *centralWidget();

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

    const float mZoomStepSize;

    float mMeterToPixelRatio;
    QPointF mOrigin;
};

#endif // CONTROLLER_H
