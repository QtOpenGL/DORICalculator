#ifndef TOPVIEWWIDGET_H
#define TOPVIEWWIDGET_H

#include <Core/Controller.h>
#include <Dependencies/Eigen/Core>
#include <GUI/Base/Handle.h>
#include <QWidget>

class TopViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopViewWidget(QWidget *parent = nullptr);
    void setParameters(Dori::Core::Controller::TopViewWidgetParamaters *newParameters);

signals:
    void dirty();
    void zoom(int);

public slots:
    void init();
    void refresh();
    QPointF mapFrom3d(Eigen::Vector3f vector);
    Eigen::Vector3f mapFrom2d(QPointF point);

private:
    void updateHandles();

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    Handle mFovWidthHandleTop;
    Handle mFovWidthHandleBottom;
    Handle mTargetHandle;

    QPen mDashedPen;
    QPen mSolidPen;
    QPoint mOldMousePosition;

    Dori::Core::Controller::TopViewWidgetParamaters *mParameters;

    QFont mLabelFont;
    QColor mLabelColor;
};

#endif // TOPVIEWWIDGET_H
