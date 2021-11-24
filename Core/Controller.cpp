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
    mLogic.calculate();

    // SideViewWidgetParameters

    mSideViewWidgetParameters->camera.tiltAngle = mLogicOutputParameters->camera.tiltAngle;
    mSideViewWidgetParameters->camera.height = mLogicOutputParameters->camera.height;
    mSideViewWidgetParameters->camera.position = mSideViewWidget->mapFrom3d(0, mLogicOutputParameters->camera.height);
    mSideViewWidgetParameters->target.height = mLogicOutputParameters->target.height;
    mSideViewWidgetParameters->target.distance = mLogicOutputParameters->target.distance;
    mSideViewWidgetParameters->target.position = mSideViewWidget->mapFrom3d(mLogicOutputParameters->target.distance, mLogicOutputParameters->target.height);
    mSideViewWidgetParameters->lowerBoundary.height = mLogicOutputParameters->lowerBoundary.height;
    mSideViewWidgetParameters->lowerBoundary.distance = mLogicOutputParameters->lowerBoundary.distance;
    mSideViewWidgetParameters->lowerBoundary.position = mSideViewWidget->mapFrom3d(mLogicOutputParameters->lowerBoundary.distance, mLogicOutputParameters->lowerBoundary.height);

    mSideViewWidgetParameters->points[0] = mSideViewWidget->mapFrom3d(mLogicOutputParameters->frustum.oppositeBisectorRay);
    mSideViewWidgetParameters->points[1] = mSideViewWidget->mapFrom3d(mLogicOutputParameters->frustum.bisectorRay);
    mSideViewWidgetParameters->points[2] = mSideViewWidget->mapFrom3d(mLogicOutputParameters->frustum.bottomVertices[0]);
    mSideViewWidgetParameters->points[3] = mSideViewWidget->mapFrom3d(mLogicOutputParameters->frustum.bottomVertices[1]);

    QPolygonF roi;
    roi.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->target.distance, mLogicOutputParameters->target.height));
    roi.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->target.distance, mLogicOutputParameters->lowerBoundary.height));
    roi.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->lowerBoundary.distance, mLogicOutputParameters->lowerBoundary.height));
    roi.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->lowerBoundary.distance, mLogicOutputParameters->target.height));

    for (RegionNames name : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING}) {
        if (!mLogicOutputParameters->regions[name].visible) {
            mSideViewWidgetParameters->regions[name].visible = false;
            continue;
        }

        QPolygonF region;

        region.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].bottomVertices[0]));
        region.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[0]));
        region.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[2]));
        region.append(mSideViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].bottomVertices[2]));

        region = region.intersected(roi);
        mSideViewWidgetParameters->regions[name].region = region;
        mSideViewWidgetParameters->regions[name].visible = !region.isEmpty();
    }

    // TopViewWidgetParameters
    {
        mTopViewWidgetParameters->targetDistance = mLogicOutputParameters->target.distance;
        mTopViewWidgetParameters->fovWidth = 0;

        for (int i = 0; i < 4; ++i) {
            mTopViewWidgetParameters->ground[i] = mTopViewWidget->mapFrom3d(mLogicOutputParameters->frustum.bottomVertices[i]);
            mTopViewWidgetParameters->target[i] = mTopViewWidget->mapFrom3d(mLogicOutputParameters->target.intersections[i]);
            mTopViewWidgetParameters->lowerBoundary[i] = mTopViewWidget->mapFrom3d(mLogicOutputParameters->lowerBoundary.intersections[i]);
        }

        QPolygonF roi;
        roi.append(mTopViewWidgetParameters->target[0]);
        roi.append(mTopViewWidgetParameters->lowerBoundary[1]);
        roi.append(mTopViewWidgetParameters->lowerBoundary[2]);
        roi.append(mTopViewWidgetParameters->target[3]);

        for (RegionNames name : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING}) {
            if (!mLogicOutputParameters->regions[name].visible) {
                mTopViewWidgetParameters->regions[name].visible = false;
                continue;
            }

            QPolygonF region;

            region.append(mTopViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[0]));
            region.append(mTopViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[1]));
            region.append(mTopViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[2]));
            region.append(mTopViewWidget->mapFrom3d(mLogicOutputParameters->regions[name].topVertices[3]));

            region = region.intersected(roi);
            mTopViewWidgetParameters->regions[name].region = region;
            mTopViewWidgetParameters->regions[name].visible = !region.isEmpty();
        }
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
        mLogicInputParameters->camera.height = mSideViewWidgetParameters->camera.height;
        mLogicInputParameters->target.height = mSideViewWidgetParameters->target.height;
        mLogicInputParameters->target.distance = mSideViewWidgetParameters->target.distance;
        mLogicInputParameters->lowerBoundary.height = mSideViewWidgetParameters->lowerBoundary.height;
    } else if (sender == mTopViewWidget) {
        mLogicInputParameters->target.distance = mTopViewWidgetParameters->targetDistance;
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
    mLogicInputParameters = new Logic::Parameters;
    mLogicInputParameters->camera.height = 5;
    mLogicInputParameters->target.height = 2;
    mLogicInputParameters->target.distance = 5;
    mLogicInputParameters->lowerBoundary.height = 0;
    mLogicInputParameters->lowerBoundary.distance = 0;
    mLogicInputParameters->frustum.horizontalFov = 60;

    mLogicInputParameters->frustum.zNear = 1.0;
    mLogicInputParameters->frustum.zFar = 1000;

    mLogicInputParameters->camera.sensor.width = 1920.0f;
    mLogicInputParameters->camera.sensor.height = 1080.0f;
    mLogicInputParameters->camera.sensor.aspectRatio = 1920.0f / 1080.0f;

    mLogicOutputParameters = new Logic::Parameters;

    mLogic.setInputParameters(mLogicInputParameters);
    mLogic.setOutputParameters(mLogicOutputParameters);

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
