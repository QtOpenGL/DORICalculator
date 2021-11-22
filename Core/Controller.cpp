#include "Controller.h"

#include <Core/Enums.h>
#include <GUI/Widgets/CentralWidget.h>
#include <GUI/Widgets/SideViewWidget.h>
#include <GUI/Widgets/TopViewWidget.h>

#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Logic::getInstance())
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

    for (EdgeNames name : {OPPOSITE_BISECTOR, BISECTOR, V1, V2}) {
        mSideViewWidgetParameters->points[name] = mSideViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[name]);
    }

    for (ZoneNames zone : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING, DEAD_ZONE}) {
        if (!mLogicParameters->zones[zone].visible) {
            mSideViewWidgetParameters->zones[zone].paint = false;
            continue;
        }

        mSideViewWidgetParameters->zones[zone].paint = true;
        mSideViewWidgetParameters->zones[zone].vertices[0] = mSideViewWidget->mapFrom3d(mLogicParameters->zones[zone].bottomVertices[0]);
        mSideViewWidgetParameters->zones[zone].vertices[1] = mSideViewWidget->mapFrom3d(mLogicParameters->zones[zone].topVertices[0]);
        mSideViewWidgetParameters->zones[zone].vertices[2] = mSideViewWidget->mapFrom3d(mLogicParameters->zones[zone].topVertices[2]);
        mSideViewWidgetParameters->zones[zone].vertices[3] = mSideViewWidget->mapFrom3d(mLogicParameters->zones[zone].bottomVertices[2]);
    }

    // TopViewWidgetParameters
    mTopViewWidgetParameters->targetDistance = mLogicParameters->target.distance;
    mTopViewWidgetParameters->fovWidth = 0;

    for (int i = 0; i < 4; ++i) {
        mTopViewWidgetParameters->ground[i] = mTopViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[i + 2]);
        mTopViewWidgetParameters->target[i] = mTopViewWidget->mapFrom3d(mLogicParameters->target.intersections[i]);
        mTopViewWidgetParameters->lowerBoundary[i] = mTopViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.intersections[i]);
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
        mLogicParameters->target.distance = mTopViewWidgetParameters->targetDistance;
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

void Controller::onPan(int x, int y)
{
    setOrigin(QPointF(mOrigin.x() + x, mOrigin.y() + y));
}

void Controller::init()
{
    mLogicParameters = new Logic::Parameters;
    mLogicParameters->camera.height = 5;
    mLogicParameters->target.height = 2;
    mLogicParameters->target.distance = 5;
    mLogicParameters->lowerBoundary.height = 0;
    mLogicParameters->lowerBoundary.distance = 0;
    mLogicParameters->frustum.horizontalFov = 60;

    mLogicParameters->frustum.zNear = 0;
    mLogicParameters->frustum.zFar = 1000;

    mLogicParameters->camera.sensor.width = 1920.0f;
    mLogicParameters->camera.sensor.height = 1080.0f;
    mLogicParameters->camera.sensor.aspectRatio = 1920.0f / 1080.0f;

    mCentralWidget = new CentralWidget;
    mCentralWidget->init();

    mSideViewWidget = mCentralWidget->sideViewWidget();
    mSideViewWidgetParameters = new SideViewWidgetParameters;
    mSideViewWidget->setParameters(mSideViewWidgetParameters);

    mTopViewWidget = mCentralWidget->topViewWidget();
    mTopViewWidgetParameters = new TopViewWidgetParamaters;
    mTopViewWidget->setParameters(mTopViewWidgetParameters);

    mAxisWidget = mCentralWidget->axisWidget();

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
    connect(mSideViewWidget, &SideViewWidget::zoom, this, &Controller::onZoom);
    connect(mSideViewWidget, &SideViewWidget::pan, this, &Controller::onPan);

    connect(mTopViewWidget, &TopViewWidget::dirty, this, &Controller::onDirty);
    connect(mTopViewWidget, &TopViewWidget::zoom, this, &Controller::onZoom);
    connect(mTopViewWidget, &TopViewWidget::pan, this, &Controller::onPan);

    setMeterToPixelRatio(10);
    setOrigin(mOrigin);

    update();
}

void Controller::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (newMeterToPixelRatio < 4.0 || newMeterToPixelRatio > 128.0f) {
        return;
    }

    mMeterToPixelRatio = newMeterToPixelRatio;

    mSideViewWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mTopViewWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mAxisWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mAxisWidget->refresh();

    update();
}

void Controller::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;

    mSideViewWidget->setOrigin(newOrigin);
    mAxisWidget->setOrigin(newOrigin);
    mTopViewWidget->setOrigin(newOrigin);

    mAxisWidget->refresh();
    update();
}
