#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <QObject>
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
        QPointF position;
    };

    struct Target
    {
        float height;
        float distance;
        QPointF position;
    };

    struct LowerBoundary : Target
    {};

    struct Zone
    {
        QPointF vertices[4];
        bool paint;
    };

    struct SideViewWidgetParameters
    {
        Camera camera;
        Target target;
        LowerBoundary lowerBoundary;
        QPointF points[4]; // OPPOSITE_BISECTOR, BISECTOR, V1, V2
        Zone zones[7];
    };

    struct TopViewWidgetParamaters
    {
        float targetDistance;
        float fovWidth;
        QPointF ground[4];
        QPointF target[4];
        QPointF lowerBoundary[4];
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

    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    CentralWidget *mCentralWidget;
    AxisWidget *mAxisWidget;

    const float mZoomStepSize;

    float mMeterToPixelRatio;
    QPointF mOrigin;
};

#endif // CONTROLLER_H
