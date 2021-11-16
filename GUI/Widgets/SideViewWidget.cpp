#include "SideViewWidget.h"

#include <QMouseEvent>
#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent)
    : ViewWidget(parent)
    , mTargetHeightHandle(this)
    , mTargetDistanceHandle(this)
    , mCameraHeightHandle(this)
{
    initAxisDrawerParameters();
    mCameraHeight = 100;
    mTargetDistance = 50;
    mTargetHeight = 50;

    mOrigin.setX(width() / 2.0);
    mOrigin.setY(height() / 2.0);

    // Target height handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHeightHandle.setPen(pen);

        mTargetHeightHandle.setBrush(QColor(255, 0, 0));
        mTargetHeightHandle.setHoveredBrush(QColor(255, 255, 0));
        mTargetHeightHandle.setPressedBrush(QColor(0, 255, 0));

        mTargetHeightHandle.setSize(40, 40);
        mTargetHeightHandle.setCenter(origin().x() + mTargetDistance, origin().y() - mTargetHeight);
    }

    // Target distance handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetDistanceHandle.setPen(pen);

        mTargetDistanceHandle.setBrush(QColor(0, 0, 255));
        mTargetDistanceHandle.setHoveredBrush(QColor(0, 255, 255));
        mTargetDistanceHandle.setPressedBrush(QColor(255, 0, 255));

        mTargetDistanceHandle.setSize(40, 40);
        mTargetDistanceHandle.setCenter(origin().x() + mTargetDistance, origin().y());
    }

    setMouseTracking(true);
}

void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    //    QBrush brush;
    //    brush.setStyle(Qt::BrushStyle::SolidPattern);
    //    brush.setColor(QColor(180, 180, 180));
    //    painter.fillRect(0, 0, width(), height(), brush);

    mAxisDrawer.draw();
    mTargetDistanceHandle.draw();
    mTargetHeightHandle.draw();
}

void SideViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (mTargetHeightHandle.contains(event->pos())) {
        mTargetHeightHandle.setPressed(true);
    }

    mOldMousePosition = event->pos();

    update();
}

void SideViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setHovered(mTargetHeightHandle.contains(event->pos()));

    if (mTargetHeightHandle.pressed()) {
        mTargetHeightHandle.translate(0, (event->pos() - mOldMousePosition).y());
    }

    mOldMousePosition = event->pos();
    update();
}

void SideViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setPressed(false);
    mOldMousePosition = event->pos();
    update();
}

void SideViewWidget::initAxisDrawerParameters()
{
    // Horizontal axis parameters
    {
        mHorizontalAxisParameters.minorTickmarkCount = 1;
        mHorizontalAxisParameters.tickmarkValueStep = 6;

        mHorizontalAxisParameters.labelStyle.pen = QColor(0, 0, 0);
        mHorizontalAxisParameters.labelStyle.font = QFont("Arial");
        mHorizontalAxisParameters.labelStyle.font.setPixelSize(9);

        mHorizontalAxisParameters.tickmarkStyle.brush = QColor(100, 100, 100);
        mHorizontalAxisParameters.tickmarkStyle.size = QSizeF(1.5, 6);

        mHorizontalAxisParameters.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mHorizontalAxisParameters.minorTickmarkStyle.size = QSizeF(1, 4);

        mHorizontalAxisParameters.pen = QPen(QColor(0, 0, 0));
        mHorizontalAxisParameters.pen.setWidthF(1);
    }

    // Vertical axis parameters
    {
        mVerticalAxisParameters.minorTickmarkCount = 1;
        mVerticalAxisParameters.tickmarkValueStep = 6;
        mVerticalAxisParameters.labelStyle.pen = QColor(0, 0, 0);
        mVerticalAxisParameters.labelStyle.font = QFont("Arial");
        mVerticalAxisParameters.labelStyle.font.setPixelSize(9);
        mVerticalAxisParameters.tickmarkStyle.brush = QColor(100, 100, 100);
        mVerticalAxisParameters.tickmarkStyle.size = QSizeF(6, 1.5);
        mVerticalAxisParameters.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mVerticalAxisParameters.minorTickmarkStyle.size = QSizeF(4, 1);
        mVerticalAxisParameters.pen = QPen(QColor(0, 0, 0));
        mVerticalAxisParameters.pen.setWidthF(1);
    }

    mValueToPixelRatio = 10;
}
