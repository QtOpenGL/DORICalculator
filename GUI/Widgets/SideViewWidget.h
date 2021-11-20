#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include "ViewWidget.h"
#include <Core/Controller.h>
#include <GUI/Base/Handle.h>
#include <QWidget>

class SideViewWidget : public ViewWidget
{
    Q_OBJECT
public:
    explicit SideViewWidget(QWidget *parent = nullptr);

    void setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters);

signals:
    void dirty();

public slots:
    void init();
    void refresh();

private:
    void initAxisDrawer();
    void initHandles();
    void updateHandles();

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    Handle mTargetHeightHandle;
    Handle mTargetDistanceHandle;
    Handle mCameraHeightHandle;

    QPen mDashedPen;
    QPen mSolidPen;
    QPoint mOldMousePosition;

    Dori::Core::Controller::SideViewWidgetParameters *mParameters;
};

#endif // SIDEVIEWWIDGET_H
