#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <QObject>
#include <QVector2D>

class SideViewWidget;
class CentralWidget;

namespace Dori {
namespace Core {
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    struct SideViewWidgetParameters
    {
        // GUI
        QPointF origin;
        float meterToPixelRatio;
        int minorTickmarkCount;
        int tickmarkPixelStep;

        // Logic
        QPointF camera;
        QPointF target;
        QPointF lowerBoundary;
        float tiltAngle;

        // Intersections
        QPointF intersections[3]; // BISECTOR, V1, V2
    };
    CentralWidget *centralWidget();

    float meterToPixelRatio() const;

public slots:
    void onDirty();
    void onZoom(int);
    void init();

private:
    void calculate();
    void setMeterToPixelRatio(float newMeterToPixelRatio);

    Dori::Core::Logic &mLogic;

    Dori::Core::Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;

    SideViewWidget *mSideViewWidget;
    CentralWidget *mCentralWidget;

    float mMeterToPixelRatio;
    const float mZoomStepSize;
    const QPointF mOrigin;
};

};     // namespace Core
};     // namespace Dori
#endif // CONTROLLER_H
