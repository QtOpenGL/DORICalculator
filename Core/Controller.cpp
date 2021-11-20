#include "Controller.h"

#include <GUI/Widgets/CentralWidget.h>
#include <GUI/Widgets/SideViewWidget.h>

Dori::Core::Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Dori::Core::Logic::getInstance())
{}

CentralWidget *Dori::Core::Controller::centralWidget()
{
    return mCentralWidget;
}

void Dori::Core::Controller::calculate()
{
    *mLogicParameters = mLogic.calculate(*mLogicParameters);

    // Meter to pixel conversion
    mSideViewWidgetParameters->cameraHeight = mLogicParameters->cameraHeight;
    mSideViewWidgetParameters->targetDistance = mLogicParameters->targetDistance;
    mSideViewWidgetParameters->targetHeight = mLogicParameters->targetHeight;
    mSideViewWidgetParameters->tiltAngle = mLogicParameters->tiltAngle;
}

void Dori::Core::Controller::onDirty()
{
    // Pixel to meter conversion
    mLogicParameters->cameraHeight = mSideViewWidgetParameters->cameraHeight;
    mLogicParameters->targetHeight = mSideViewWidgetParameters->targetHeight;
    mLogicParameters->targetDistance = mSideViewWidgetParameters->targetDistance;

    calculate();
    mSideViewWidget->refresh();
}

void Dori::Core::Controller::init()
{
    mLogicParameters = new Logic::Parameters;
    mLogicParameters->cameraHeight = 100;
    mLogicParameters->targetHeight = 100;
    mLogicParameters->targetDistance = 200;
    mLogicParameters->lowerBoundaryHeight = 0;
    mLogicParameters->horizontalFov = 60;
    mLogicParameters->aspectRatio = 16.0f / 9.0f;
    mLogicParameters->frustum.zNear = 0;
    mLogicParameters->frustum.zFar = 1000;

    mSideViewWidgetParameters = new SideViewWidgetParameters;

    calculate();

    mCentralWidget = new CentralWidget;
    mCentralWidget->init();

    mSideViewWidget = mCentralWidget->sideViewWidget();

    mSideViewWidget->setParameters(mSideViewWidgetParameters);
    mSideViewWidget->setOrigin(QPointF(400, 200));
    mSideViewWidget->setValueToPixelRatio(1);
    mSideViewWidget->init();

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
}
