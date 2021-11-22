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

    void setParameters(Controller::TopViewWidgetParamaters *newParameters);
    void setOrigin(QPointF newOrigin);
    void setMeterToPixelRatio(float newMeterToPixelRatio);

signals:
    void dirty();
    void pan(int x, int y);
    void zoom(int);

public slots:
    void refresh();
    QPointF mapFrom3d(Eigen::Vector3f vector);
    Eigen::Vector3f mapFrom2d(QPointF point);

private:
    void updateHandles();

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *event) override;

    Controller::TopViewWidgetParamaters *mParameters;
    QPointF mOrigin;
    float mMeterToPixelRatio;

    Handle mFovWidthHandleTop;
    Handle mFovWidthHandleBottom;
    Handle mTargetHandle;
    Handle mCameraHandle;

    QBrush mCrossedPatternBursh;
    QPen mDashedPen;
    QPen mSolidPen;

    QPoint mOldMousePosition;

    QFont mLabelFont;
    QColor mLabelColor;

    bool mMousePressedOnCanvas;
};

#endif // TOPVIEWWIDGET_H
