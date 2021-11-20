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

public slots:
    void init();
    void refresh();
    QPointF mapFromCartesian(Eigen::Vector3f vector);
    Eigen::Vector3f mapFromGui(QPointF point);

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
};

#endif // SIDEVIEWWIDGET_H
