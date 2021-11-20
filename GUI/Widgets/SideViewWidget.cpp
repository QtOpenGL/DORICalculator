#include "SideViewWidget.h"

#include <QMouseEvent>
#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent)
    : ViewWidget(parent)
    , mTargetHeightHandle(this)
    , mTargetDistanceHandle(this)
    , mCameraHeightHandle(this)
{
    setMouseTracking(true);
}

void SideViewWidget::init()
{
    mOrigin.setX(width() / 2.0);
    mOrigin.setY(height() / 2.0);

    mValueToPixelRatio = 10;

    initAxisDrawer();
    initHandles();
    refresh();
}

void SideViewWidget::refresh()
{
    updateHandles();
    update();
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
    mCameraHeightHandle.draw();
}

void SideViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (mTargetHeightHandle.contains(event->pos())) {
        mTargetHeightHandle.setPressed(true);
    } else if (mTargetDistanceHandle.contains(event->pos())) {
        mTargetDistanceHandle.setPressed(true);
    } else if (mCameraHeightHandle.contains(event->pos())) {
        mCameraHeightHandle.setPressed(true);
    }

    mOldMousePosition = event->pos();
    update();
}

void SideViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setHovered(mTargetHeightHandle.contains(event->pos()));
    mTargetDistanceHandle.setHovered(mTargetDistanceHandle.contains(event->pos()));
    mCameraHeightHandle.setHovered(mCameraHeightHandle.contains(event->pos()));

    bool isDirty = false;
    if (mTargetHeightHandle.pressed()) {
        isDirty = true;
        mParameters->targetHeight -= (event->pos() - mOldMousePosition).y();
    }

    if (mTargetDistanceHandle.pressed()) {
        isDirty = true;
        mParameters->targetDistance += (event->pos() - mOldMousePosition).x();
    }

    if (mCameraHeightHandle.pressed()) {
        isDirty = true;
        mParameters->cameraHeight -= (event->pos() - mOldMousePosition).y();
    }

    mOldMousePosition = event->pos();

    update();

    if (isDirty)
        emit dirty();
}

void SideViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setPressed(false);
    mTargetDistanceHandle.setPressed(false);
    mCameraHeightHandle.setPressed(false);

    mOldMousePosition = event->pos();
    update();
}

void SideViewWidget::setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters)
{
    mParameters = newParameters;
}

void SideViewWidget::initAxisDrawer()
{
    // HORIZONTAL AXIS PARAMETERS
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

    // VERTICAL AXIS PARAMETERS
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
}

void SideViewWidget::initHandles()
{
    // TARGET HEIGHT HANDLE
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHeightHandle.setPen(pen);
        mTargetHeightHandle.setBrush(QColor(255, 0, 0));
        mTargetHeightHandle.setHoveredBrush(QColor(255, 255, 0));
        mTargetHeightHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetHeightHandle.setSize(10, 10);
    }

    // TARGET DISTANCE HANDLE
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetDistanceHandle.setPen(pen);
        mTargetDistanceHandle.setBrush(QColor(0, 0, 255));
        mTargetDistanceHandle.setHoveredBrush(QColor(255, 255, 0));
        mTargetDistanceHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetDistanceHandle.setSize(10, 10);
    }

    // CAMERA HEIGHT HANDLE
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mCameraHeightHandle.setPen(pen);
        mCameraHeightHandle.setBrush(QColor(255, 128, 0));
        mCameraHeightHandle.setHoveredBrush(QColor(255, 255, 0));
        mCameraHeightHandle.setPressedBrush(QColor(0, 255, 0));
        mCameraHeightHandle.setSize(10, 10);
    }
}

void SideViewWidget::updateHandles()
{
    mTargetHeightHandle.setCenter(origin().x() + mParameters->targetDistance, origin().y() - mParameters->targetHeight);
    mTargetDistanceHandle.setCenter(origin().x() + mParameters->targetDistance, origin().y());
    mCameraHeightHandle.setCenter(origin().x(), origin().y() - mParameters->cameraHeight);
}
