#include "Controller.h"
#include "Settings.h"
#include <GUI/Widgets/SideViewWidget.h>

Dori::Core::Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Dori::Core::Logic::getInstance())
{}

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
    *mLogicParameters = Settings::getInstance().defaultParameters();
    mSideViewWidgetParameters = new SideViewWidgetParameters;
    calculate();

    mSideViewWidget->setParameters(mSideViewWidgetParameters);
    mSideViewWidget->init();
}

void Dori::Core::Controller::setSideViewWidget(SideViewWidget *newSideViewWidget)
{
    mSideViewWidget = newSideViewWidget;
}
