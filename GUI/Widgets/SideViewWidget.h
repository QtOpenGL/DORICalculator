#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include "AxisWidget.h"

#include <Core/Controller.h>
#include <GUI/Base/Handle.h>
#include <QWidget>

class SideViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideViewWidget(QWidget *parent = nullptr);
    void setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters);

    void setOrigin(QPointF newOrigin);
    void setMeterToPixelRatio(float newMeterToPixelRatio);

signals:
    void dirty();
    void zoom(int);
    void pan(int x, int y);

public slots:
    void refresh();
    QPointF mapFrom3d(Eigen::Vector3f vector);
    Eigen::Vector3f mapFrom2d(QPointF point);

private:
    void updateHandles();

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    Dori::Core::Controller::SideViewWidgetParameters *mParameters;

    Handle mTargetHeightHandle;
    Handle mTargetDistanceHandle;
    Handle mCameraHeightHandle;

    QPoint mOldMousePosition;
    bool mMousePressedOnCanvas;

    QPen mDashedPen;
    QPen mSolidPen;
    QFont mLabelFont;
    QColor mLabelColor;

    QPointF mOrigin;
    float mMeterToPixelRatio;
};

#endif // SIDEVIEWWIDGET_H
