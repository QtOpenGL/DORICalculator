#include "Controller.h"

#include <GUI/Widgets/CentralWidget.h>
#include <GUI/Widgets/SideViewWidget.h>

#include <QDebug>

namespace Dori {
namespace Core {
Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Dori::Core::Logic::getInstance())
    , mZoomStepSize(2.0f)
    , mOrigin(256, 256)
{}

CentralWidget *Controller::centralWidget()
{
    return mCentralWidget;
}

void Controller::calculate()
{
    *mLogicParameters = mLogic.calculate(*mLogicParameters);

    mSideViewWidgetParameters->camera = mSideViewWidget->mapFromCartesian(mLogicParameters->camera);
    mSideViewWidgetParameters->target = mSideViewWidget->mapFromCartesian(mLogicParameters->target);
    mSideViewWidgetParameters->tiltAngle = mLogicParameters->tiltAngle;

    for (Logic::EdgeNames name : {Logic::BISECTOR, Logic::V1, Logic::V2}) {
        mSideViewWidgetParameters->intersections[name] = mSideViewWidget->mapFromCartesian(mLogicParameters->frustum.bottomVertices[name]);
    }
}

void Controller::onDirty()
{
    mLogicParameters->camera = mSideViewWidget->mapFromGui(mSideViewWidgetParameters->camera);
    mLogicParameters->target = mSideViewWidget->mapFromGui(mSideViewWidgetParameters->target);
    mLogicParameters->lowerBoundary = mSideViewWidget->mapFromGui(mSideViewWidgetParameters->lowerBoundary);

    calculate();
    mSideViewWidget->refresh();
}

void Controller::onZoom(int i)
{
    if (i < 0) {
        setMeterToPixelRatio(mZoomStepSize * mMeterToPixelRatio);
    } else if (i > 0) {
        setMeterToPixelRatio(mMeterToPixelRatio / mZoomStepSize);
    }
}

void Controller::init()
{
    mLogicParameters = new Logic::Parameters;
    mLogicParameters->camera = Eigen::Vector3f(0, 0, 15);
    mLogicParameters->target = Eigen::Vector3f(20, 0, 5);
    mLogicParameters->lowerBoundary = Eigen::Vector3f(0, 0, 0);
    mLogicParameters->frustum.horizontalFov = 60;
    mLogicParameters->frustum.aspectRatio = 16.0f / 9.0f;
    mLogicParameters->frustum.zNear = 0;
    mLogicParameters->frustum.zFar = 1000;

    mCentralWidget = new CentralWidget;
    mCentralWidget->init();

    mSideViewWidgetParameters = new SideViewWidgetParameters;
    mSideViewWidget = mCentralWidget->sideViewWidget();
    mSideViewWidget->setParameters(mSideViewWidgetParameters);
    mSideViewWidgetParameters->origin = mOrigin;
    mSideViewWidgetParameters->minorTickmarkCount = 1;
    mSideViewWidgetParameters->tickmarkPixelStep = 50;
    setMeterToPixelRatio(10);
    calculate();

    mSideViewWidget->init();

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
    connect(mSideViewWidget, &SideViewWidget::zoom, this, &Controller::onZoom);
}

void Controller::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (newMeterToPixelRatio < 4.0 || newMeterToPixelRatio > 128.0f) {
        return;
    }

    mMeterToPixelRatio = newMeterToPixelRatio;

    mSideViewWidgetParameters->meterToPixelRatio = mMeterToPixelRatio;
    calculate();

    mSideViewWidget->refresh();
}

} // namespace Core
} // namespace Dori
