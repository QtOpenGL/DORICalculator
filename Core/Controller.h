#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"
#include <QObject>

class SideViewWidget;
class CentralWidget;

namespace Dori {
namespace Core {
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    enum VectorType { LOWER, BISECTOR, UPPER };
    enum IntersectionType { EMPTY, NON_EMPTY };

    struct IntersectionPoint
    {
        QPointF point;
        IntersectionType type;
    };

    struct SideViewWidgetParameters
    {
        float cameraHeight;
        float targetHeight;
        float targetDistance;
        float tiltAngle;
        float lowerBoundaryDistance;
        float lowerBoundaryHeight;
    };
    CentralWidget *centralWidget();

public slots:
    void onDirty();
    void init();

private:
    void calculate();

    Dori::Core::Logic &mLogic;

    Dori::Core::Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;

    SideViewWidget *mSideViewWidget;
    CentralWidget *mCentralWidget;
};

};     // namespace Core
};     // namespace Dori
#endif // CONTROLLER_H
