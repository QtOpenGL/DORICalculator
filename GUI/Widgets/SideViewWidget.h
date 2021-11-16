#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include "ViewWidget.h"

#include <GUI/Base/Handle.h>
#include <QWidget>

class SideViewWidget : public ViewWidget
{
public:
    explicit SideViewWidget(QWidget *parent = nullptr);

private:
    void initAxisDrawerParameters();

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    Handle mTargetHeightHandle, mTargetDistanceHandle;
    Handle mCameraHeightHandle;

    QPen mDashedPen, mSolidPen;

    float mCameraHeight;
    float mTargetDistance, mTargetHeight;

    QPoint mOldMousePosition;
};

#endif // SIDEVIEWWIDGET_H
