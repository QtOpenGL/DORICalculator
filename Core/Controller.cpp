#include "Controller.h"

#include <GUI/Widgets/CentralWidget.h>
#include <GUI/Widgets/SideViewWidget.h>
#include <GUI/Widgets/TopViewWidget.h>

#include <QDebug>

namespace Dori {
namespace Core {
Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Dori::Core::Logic::getInstance())
    , mZoomStepSize(2.0f)
    , mOrigin(128, 368)
{}

CentralWidget *Controller::centralWidget()
{
    return mCentralWidget;
}

void Controller::calculate()
{
    *mLogicParameters = mLogic.calculate(*mLogicParameters);

    // SideViewWidgetParameters
    mSideViewWidgetParameters->camera.tiltAngle = mLogicParameters->camera.tiltAngle;
    mSideViewWidgetParameters->camera.height = mLogicParameters->camera.height;
    mSideViewWidgetParameters->camera.position = mSideViewWidget->mapFrom3d(Eigen::Vector3f(0, 0, mLogicParameters->camera.height));
    mSideViewWidgetParameters->target.height = mLogicParameters->target.height;
    mSideViewWidgetParameters->target.distance = mLogicParameters->target.distance;
    mSideViewWidgetParameters->target.position = mSideViewWidget->mapFrom3d(Eigen::Vector3f(mLogicParameters->target.distance, 0, mLogicParameters->target.height));
    mSideViewWidgetParameters->lowerBoundary.height = mLogicParameters->lowerBoundary.height;
    mSideViewWidgetParameters->lowerBoundary.distance = mLogicParameters->lowerBoundary.distance;
    mSideViewWidgetParameters->lowerBoundary.position = mSideViewWidget->mapFrom3d(
        Eigen::Vector3f(mLogicParameters->lowerBoundary.distance, 0, mLogicParameters->lowerBoundary.height));

    for (Logic::EdgeNames name : {Logic::OPPOSITE_BISECTOR, Logic::BISECTOR, Logic::V1, Logic::V2}) {
        mSideViewWidgetParameters->points[name] = mSideViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[name]);
    }

    // TopViewWidgetParameters
    mTopViewWidgetParameters->targetDistance = mLogicParameters->target.distance;
    mTopViewWidgetParameters->fovWidth = 0;

    for (Logic::EdgeNames name : {Logic::V1, Logic::V2, Logic::V3, Logic::V4}) {
        mTopViewWidgetParameters->ground[name - Logic::V1] = mTopViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[name]);
        mTopViewWidgetParameters->target[name - Logic::V1] = mTopViewWidget->mapFrom3d(mLogicParameters->target.intersections[name - Logic::V1]);
        mTopViewWidgetParameters->lowerBoundary[name - Logic::V1] = mTopViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.intersections[name - Logic::V1]);
    }
}

void Controller::update()
{
    calculate();
    mSideViewWidget->refresh();
    mTopViewWidget->refresh();
}

void Controller::onDirty()
{
    QObject *sender = QObject::sender();
    if (sender == mSideViewWidget) {
        mLogicParameters->camera.height = mSideViewWidgetParameters->camera.height;
        mLogicParameters->target.height = mSideViewWidgetParameters->target.height;
        mLogicParameters->target.distance = mSideViewWidgetParameters->target.distance;
        mLogicParameters->lowerBoundary.height = mSideViewWidgetParameters->lowerBoundary.height;
    } else if (sender == mTopViewWidget) {
    }

    update();
}

void Controller::onZoom(int i)
{
    if (i < 0) {
        setMeterToPixelRatio(mZoomStepSize * mMeterToPixelRatio);
    } else if (i > 0) {
        setMeterToPixelRatio(mMeterToPixelRatio / mZoomStepSize);
    }
}

void Controller::onPan(float x, float y)
{
    setOrigin(QPointF(mOrigin.x() + x, mOrigin.y() + y));
}

void Controller::init()
{
    mLogicParameters = new Logic::Parameters;
    mLogicParameters->camera.height = 15;
    mLogicParameters->target.height = 5;
    mLogicParameters->target.distance = 20;
    mLogicParameters->lowerBoundary.height = 0;
    mLogicParameters->lowerBoundary.distance = 0;
    mLogicParameters->frustum.horizontalFov = 60;
    mLogicParameters->frustum.aspectRatio = 16.0f / 9.0f;
    mLogicParameters->frustum.zNear = 0;
    mLogicParameters->frustum.zFar = 1000;

    mCentralWidget = new CentralWidget;
    mCentralWidget->init();

    mSideViewWidget = mCentralWidget->sideViewWidget();
    mSideViewWidgetParameters = new SideViewWidgetParameters;
    mSideViewWidget->setParameters(mSideViewWidgetParameters);

    mSideViewWidgetParameters->minorTickmarkCount = 1;
    mSideViewWidgetParameters->tickmarkPixelStep = 50;

    mTopViewWidget = mCentralWidget->topViewWidget();
    mTopViewWidgetParameters = new TopViewWidgetParamaters;
    mTopViewWidget->setParameters(mTopViewWidgetParameters);

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
    connect(mSideViewWidget, &SideViewWidget::zoom, this, &Controller::onZoom);
    connect(mSideViewWidget, &SideViewWidget::pan, this, &Controller::onPan);

    setMeterToPixelRatio(10);
    setOrigin(mOrigin);

    mSideViewWidget->init();
    mTopViewWidget->init();
    update();
}

void Controller::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (newMeterToPixelRatio < 4.0 || newMeterToPixelRatio > 128.0f) {
        return;
    }

    mMeterToPixelRatio = newMeterToPixelRatio;

    mSideViewWidgetParameters->meterToPixelRatio = mMeterToPixelRatio;
    mTopViewWidgetParameters->meterToPixelRatio = mMeterToPixelRatio;

    update();
}

void Controller::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;

    mSideViewWidgetParameters->origin = newOrigin;
    mTopViewWidgetParameters->origin = newOrigin;

    update();
}

} // namespace Core
} // namespace Dori
