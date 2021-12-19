#include "CentralWidget.h"

#include <QGridLayout>

#include <Core/Controller.h>
#include <OpenGL/OpenGLWidget.h>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{}

void CentralWidget::init()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 6);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(mAxisWidget, 0, 0);
    layout->addWidget(mSideViewWidget, 0, 0);

    mainLayout->addLayout(layout, 0, 1, 2, 1);

    mainLayout->addWidget(mTopViewWidget, 2, 1, 3, 1);

    mainLayout->addWidget(mCameraWidget, 0, 0);
    mainLayout->addWidget(mTargetWidget, 1, 0);
    mainLayout->addWidget(mLowerBoundaryWidget, 2, 0);
    mainLayout->addWidget(mCursorPositionWidget, 3, 0);
    mainLayout->addItem(new QSpacerItem(-1, -1, QSizePolicy::Ignored, QSizePolicy::Expanding), 4, 0);

    setLayout(mainLayout);
}

void CentralWidget::setSideViewWidget(SideViewWidget *newSideViewWidget)
{
    mSideViewWidget = newSideViewWidget;
}

void CentralWidget::setTopViewWidget(TopViewWidget *newTopViewWidget)
{
    mTopViewWidget = newTopViewWidget;
}

void CentralWidget::setAxisWidget(AxisWidget *newAxisWidget)
{
    mAxisWidget = newAxisWidget;
}

void CentralWidget::setCameraWidget(CameraWidget *newCameraWidget)
{
    mCameraWidget = newCameraWidget;
}

void CentralWidget::setTargetWidget(TargetWidget *newTargetWidget)
{
    mTargetWidget = newTargetWidget;
}

void CentralWidget::setLowerBoundaryWidget(LowerBoundaryWidget *newLowerBoundaryWidget)
{
    mLowerBoundaryWidget = newLowerBoundaryWidget;
}

void CentralWidget::setCursorPositionWidget(CursorPositionWidget *newCursorPositionWidget)
{
    mCursorPositionWidget = newCursorPositionWidget;
}
