#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <QObject>
#include <QVector2D>

class SideViewWidget;
class TopViewWidget;
class CentralWidget;

namespace Dori {
namespace Core {
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

    struct SideViewWidgetParameters
    {
        QPointF origin;
        float meterToPixelRatio;
        int minorTickmarkCount;
        int tickmarkPixelStep;

        Camera camera;
        Target target;
        LowerBoundary lowerBoundary;

        // Ground intersections
        QPointF points[4]; // OPPOSITE_BISECTOR, BISECTOR, V1, V2
    };

    struct TopViewWidgetParamaters
    {
        float targetDistance;
        QPointF origin;

        // Intersections
        QPointF ground[4];
        QPointF target[2];
        QPointF lowerBounddary[2];
    };

    CentralWidget *centralWidget();

    float meterToPixelRatio() const;

public slots:
    void onDirty();
    void onZoom(int);
    void onPan(float x, float y);
    void init();

private:
    void calculate();
    void setMeterToPixelRatio(float newMeterToPixelRatio);

    Dori::Core::Logic &mLogic;

    Dori::Core::Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;
    TopViewWidgetParamaters *mTopViewWidgetParameters;

    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    CentralWidget *mCentralWidget;

    float mMeterToPixelRatio;
    const float mZoomStepSize;
    QPointF mOrigin;
};

};     // namespace Core
};     // namespace Dori
#endif // CONTROLLER_H
