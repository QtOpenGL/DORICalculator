#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include <Core/Controller.h>
#include <GUI/Base/Handle.h>
#include <QWidget>

class SideViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideViewWidget(QWidget *parent = nullptr);
    void setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters);

signals:
    void dirty();
    void zoom(int);
    void pan(float x, float y);

public slots:
    void init();
    void refresh();
    QPointF mapFrom3d(Eigen::Vector3f vector);
    Eigen::Vector3f mapFrom2d(QPointF point);

private:
    enum Axis { Horizontal, Vertical };

private:
    void updateHandles();
    void drawAxis(Axis);

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    Handle mTargetHeightHandle;
    Handle mTargetDistanceHandle;
    Handle mCameraHeightHandle;

    QPen mDashedPen;
    QPen mSolidPen;
    QPoint mOldMousePosition;

    Dori::Core::Controller::SideViewWidgetParameters *mParameters;

    QPen mAxisPen;
    QFont mLabelFont;
    QSizeF mTickmarkSize;
    QSizeF mMinorTickmarkSize;
    QColor mTickmarkColor;
    QColor mMinorTickmarkColor;
    QColor mLabelColor;
    QFont mBigLabelFont;

    bool mMousePressedOnCanvas;
};

#endif // SIDEVIEWWIDGET_H
