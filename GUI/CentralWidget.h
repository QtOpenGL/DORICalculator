#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "CameraWidget.h"
#include "CursorPositionWidget.h"
#include "LowerBoundaryWidget.h"
#include "SideViewWidget.h"
#include "TargetWidget.h"
#include "TopViewWidget.h"

#include <QGroupBox>
#include <QLabel>
#include <QStatusBar>
#include <QWidget>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

    void init();

    void setSideViewWidget(SideViewWidget *newSideViewWidget);
    void setTopViewWidget(TopViewWidget *newTopViewWidget);
    void setAxisWidget(AxisWidget *newAxisWidget);
    void setCameraWidget(CameraWidget *newCameraWidget);
    void setTargetWidget(TargetWidget *newTargetWidget);
    void setLowerBoundaryWidget(LowerBoundaryWidget *newLowerBoundaryWidget);
    void setCursorPositionWidget(CursorPositionWidget *newCursorPositionWidget);

private:
    CameraWidget *mCameraWidget;
    TargetWidget *mTargetWidget;
    LowerBoundaryWidget *mLowerBoundaryWidget;
    CursorPositionWidget *mCursorPositionWidget;

    SideViewWidget *mSideViewWidget;
    AxisWidget *mAxisWidget;
    TopViewWidget *mTopViewWidget;
};

#endif // CENTRALWIDGET_H
