#include "Controller.h"

#include <Core/Enums.h>
#include <GUI/CentralWidget.h>
#include <GUI/SideViewWidget.h>
#include <GUI/TopViewWidget.h>
#include <OpenGL/OpenGLWindow.h>

#include <QDebug>
#include <QtMath>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Logic::getInstance())
    , mZoomStepSize(2.0f)
    , mOrigin(368, 368)
{}

void Controller::calculate()
{
    mLogic.calculate();

    // SideViewWidgetParameters
    {
        mSideViewWidgetParameters->camera.tiltAngle = mLogicParameters->camera.tiltAngle;
        mSideViewWidgetParameters->camera.height = mLogicParameters->camera.height;
        mSideViewWidgetParameters->camera.horizontalFov = mLogicParameters->frustum.horizontalFov;
        mSideViewWidgetParameters->camera.verticalFov = mLogicParameters->frustum.verticalFov;
        mSideViewWidgetParameters->camera.position = mSideViewWidget->mapFrom3d(0, mLogicParameters->camera.height);
        mSideViewWidgetParameters->target.height = mLogicParameters->target.height;
        mSideViewWidgetParameters->target.distance = mLogicParameters->target.distance;
        mSideViewWidgetParameters->target.position = mSideViewWidget->mapFrom3d(mLogicParameters->target.distance, mLogicParameters->target.height);
        mSideViewWidgetParameters->lowerBoundary.height = mLogicParameters->lowerBoundary.height;
        mSideViewWidgetParameters->lowerBoundary.distance = mLogicParameters->lowerBoundary.distance;
        mSideViewWidgetParameters->lowerBoundary.position = mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->lowerBoundary.height);

        for (int i = 0; i < 4; ++i) {
            mSideViewWidgetParameters->ground.intersections[i] = mSideViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[i]);
            mSideViewWidgetParameters->target.intersections[i] = mSideViewWidget->mapFrom3d(mLogicParameters->target.intersections[i]);
            mSideViewWidgetParameters->lowerBoundary.intersections[i] = mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.intersections[i]);
        }

        mSideViewWidgetParameters->bisectorIntersection = mSideViewWidget->mapFrom3d(mLogicParameters->frustum.oppositeBisectorRay);
        mSideViewWidgetParameters->oppositeBisectorIntersection = mSideViewWidget->mapFrom3d(mLogicParameters->frustum.bisectorRay);

        QPolygonF roi;

        roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->target.distance, mLogicParameters->target.height));
        roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->target.distance, mLogicParameters->lowerBoundary.height));
        roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->lowerBoundary.height));
        roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->target.height));

        for (int i = 0; i < NUMBER_OF_REGIONS; ++i) {
            QPolygonF region;

            region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].bottomVertices[0]));
            region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].topVertices[0]));
            region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].topVertices[2]));
            region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].bottomVertices[2]));

            region = region.intersected(roi);
            mSideViewWidgetParameters->regions[i].region = region;
            mSideViewWidgetParameters->regions[i].visible = !region.isEmpty();
        }
    }

    // TopViewWidgetParameters
    {
        mTopViewWidgetParameters->target.distance = mLogicParameters->target.distance;
        mTopViewWidgetParameters->target.fovWidth = mLogicParameters->target.fovWidth;
        mTopViewWidgetParameters->camera.tiltAngle = mLogicParameters->camera.tiltAngle;
        mTopViewWidgetParameters->camera.height = mLogicParameters->camera.height;
        mTopViewWidgetParameters->camera.horizontalFov = mLogicParameters->frustum.horizontalFov;
        mTopViewWidgetParameters->camera.verticalFov = mLogicParameters->frustum.verticalFov;

        for (int i = 0; i < 4; ++i) {
            mTopViewWidgetParameters->ground.intersections[i] = mTopViewWidget->mapFrom3d(mLogicParameters->frustum.bottomVertices[i]);
            mTopViewWidgetParameters->target.intersections[i] = mTopViewWidget->mapFrom3d(mLogicParameters->target.intersections[i]);
            mTopViewWidgetParameters->lowerBoundary.intersections[i] = mTopViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.intersections[i]);
        }

        QPolygonF targetRoi;

        if (abs(mLogicParameters->camera.height - mLogicParameters->target.height) < 0.0001f) {
            targetRoi.append(mTopViewWidget->mapFrom3d(mTopViewWidgetParameters->target.distance, 0.5f * mTopViewWidgetParameters->target.fovWidth));
            targetRoi.append(mTopViewWidget->mapFrom3d(0, 0));
            targetRoi.append(mTopViewWidget->mapFrom3d(mTopViewWidgetParameters->target.distance, -0.5f * mTopViewWidgetParameters->target.fovWidth));
        } else if (mLogicParameters->camera.height < mLogicParameters->target.height) {
            targetRoi.append(mTopViewWidget->mapFrom3d(0, 0));
            targetRoi.append(mTopViewWidgetParameters->target.intersections[0]);
            targetRoi.append(mTopViewWidgetParameters->target.intersections[3]);
        } else {
            targetRoi.append(mTopViewWidgetParameters->target.intersections[0]);
            targetRoi.append(mTopViewWidgetParameters->target.intersections[1]);
            targetRoi.append(mTopViewWidgetParameters->target.intersections[2]);
            targetRoi.append(mTopViewWidgetParameters->target.intersections[3]);
        }

        QPolygonF lowerBoundaryRoi;
        lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[0]);
        lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[1]);
        lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[2]);
        lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[3]);

        QPolygonF roi = lowerBoundaryRoi.intersected(targetRoi);

        for (int i = 0; i < NUMBER_OF_REGIONS; ++i) {
            Eigen::Hyperplane<float, 3> plane(Eigen::Vector3f(0, 0, 1).normalized(), -mLogicParameters->lowerBoundary.height);
            QVector<Eigen::Vector3f> intersections = mLogic.findIntersection(mLogicParameters->regions[i], plane);

            QPolygonF region;
            for (int i = 0; i < intersections.size(); ++i) {
                region.append(mTopViewWidget->mapFrom3d(intersections[i]));
            }

            region = region.intersected(roi);
            mTopViewWidgetParameters->regions[i].region = region;
            mTopViewWidgetParameters->regions[i].visible = !region.isEmpty();
        }
    }

    // LeftWidgetParameters
    {
        *mLeftWidgetParameters = *mLogicParameters;
    }
}

void Controller::update()
{
    calculate();
    mSideViewWidget->refresh();
    mTopViewWidget->refresh();
    mLeftWidget->refresh();
}

void Controller::onDirty()
{
    QObject *sender = QObject::sender();
    if (sender == mSideViewWidget) {
        mLogicParameters->target.height = mLogic.validateTargetHeight(mSideViewWidgetParameters->target.height);
        mLogicParameters->target.distance = mLogic.validateTargetDistance(mSideViewWidgetParameters->target.distance);
        mLogicParameters->camera.height = mLogic.validateCameraHeight(mSideViewWidgetParameters->camera.height);
        mLogicParameters->lowerBoundary.height = mSideViewWidgetParameters->lowerBoundary.height;

    } else if (sender == mTopViewWidget) {
        mLogicParameters->target.distance = mLogic.validateTargetDistance(mTopViewWidgetParameters->target.distance);
        mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(mTopViewWidgetParameters->target.fovWidth);
        mLogicParameters->target.fovWidth = mTopViewWidgetParameters->target.fovWidth;

    } else if (sender == mLeftWidget) {
        mLogicParameters->camera.height = mLogic.validateCameraHeight(mLeftWidgetParameters->camera.height);
        mLogicParameters->camera.sensor.width = mLeftWidgetParameters->camera.sensor.width;
        mLogicParameters->camera.sensor.height = mLeftWidgetParameters->camera.sensor.height;

        mLogicParameters->frustum.zNear = mLeftWidgetParameters->frustum.zNear;
        mLogicParameters->frustum.zFar = mLeftWidgetParameters->frustum.zFar;

        mLogicParameters->target.height = mLeftWidgetParameters->target.height;
        mLogicParameters->target.distance = mLeftWidgetParameters->target.distance;

        mLogicParameters->lowerBoundary.height = mLeftWidgetParameters->lowerBoundary.height;
        mLogicParameters->lowerBoundary.distance = mLeftWidgetParameters->lowerBoundary.distance;

        if (!qFuzzyCompare(mLogicParameters->target.fovWidth, mLeftWidgetParameters->target.fovWidth))
            mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(mLeftWidgetParameters->target.fovWidth);
        else
            mLogicParameters->frustum.horizontalFov = mLeftWidgetParameters->frustum.horizontalFov;
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

    mLogicParameters->frustum.zNear = 0.1;
    mLogicParameters->frustum.zFar = 1000;

    mLogicParameters->camera.sensor.width = 1920.0f;
    mLogicParameters->camera.sensor.height = 1080.0f;
    mLogicParameters->camera.sensor.aspectRatio = 1920.0f / 1080.0f;
    mLogicParameters->target.fovWidth = 10;

    mLogic.setParameters(mLogicParameters);
    mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(10);

    mCentralWidget = new CentralWidget;
    mCentralWidget->init();

    mSideViewWidget = mCentralWidget->sideViewWidget();
    mSideViewWidgetParameters = new SideViewWidgetParameters;
    mSideViewWidget->setParameters(mSideViewWidgetParameters);

    mTopViewWidget = mCentralWidget->topViewWidget();
    mTopViewWidgetParameters = new TopViewWidgetParamaters;
    mTopViewWidget->setParameters(mTopViewWidgetParameters);

    mAxisWidget = mCentralWidget->axisWidget();

    mLeftWidgetParameters = new Logic::Parameters;
    mLeftWidget = mCentralWidget->leftWidget();
    mLeftWidget->setParameters(mLeftWidgetParameters);

    mOpenGLWindow = new OpenGLWindow;

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
    connect(mSideViewWidget, &SideViewWidget::zoom, this, &Controller::onZoom);
    connect(mSideViewWidget, &SideViewWidget::pan, this, &Controller::onPan);
    connect(mSideViewWidget, &SideViewWidget::cursorPositionChanged, mLeftWidget, &LeftWidget::onCursorPositionChanged);

    connect(mTopViewWidget, &TopViewWidget::dirty, this, &Controller::onDirty);
    connect(mTopViewWidget, &TopViewWidget::zoom, this, &Controller::onZoom);
    connect(mTopViewWidget, &TopViewWidget::pan, this, &Controller::onPan);
    connect(mTopViewWidget, &TopViewWidget::cursorPositionChanged, mLeftWidget, &LeftWidget::onCursorPositionChanged);

    connect(mLeftWidget, &LeftWidget::dirty, this, &Controller::onDirty);

    setMeterToPixelRatio(8);
    setOrigin(mOrigin);

    update();
}

void Controller::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (newMeterToPixelRatio < 2.0 || newMeterToPixelRatio > 512.0f) {
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

OpenGLWindow *Controller::openGLWindow() const
{
    return mOpenGLWindow;
}

CentralWidget *Controller::centralWidget()
{
    return mCentralWidget;
}
