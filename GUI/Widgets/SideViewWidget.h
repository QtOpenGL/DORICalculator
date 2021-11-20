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

    QPointF origin() const;
    void setOrigin(QPointF newOrigin);
    float valueToPixelRatio() const;
    void setValueToPixelRatio(float newValueToPixelRatio);
    int minorTickmarkCount() const;
    void setMinorTickmarkCount(int newMinorTickmarkCount);
    int tickmarkPixelStep() const;
    void setTickmarkPixelStep(int newTickmarkPixelStep);

signals:
    void dirty();

public slots:
    void init();
    void refresh();

private:
    void updateHandles();
    void drawHorizontalAxis();
    void drawVerticalAxis();

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    Handle mTargetHeightHandle;
    Handle mTargetDistanceHandle;
    Handle mCameraHeightHandle;

    QPointF mOrigin;
    float mValueToPixelRatio;

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
    int mMinorTickmarkCount;
    int mTickmarkPixelStep;
};

#endif // SIDEVIEWWIDGET_H
