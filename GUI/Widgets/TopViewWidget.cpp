#include "TopViewWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

TopViewWidget::TopViewWidget(QWidget *parent)
    : QWidget(parent)
{
    // General GUI
    {
        QVector<qreal> dashes;
        dashes << 2 << 2;
        mDashedPen.setDashPattern(dashes);
        mDashedPen.setStyle(Qt::DashLine);

        mSolidPen.setWidthF(1);

        mLabelFont = QFont("Arial");
        mLabelFont.setPixelSize(11);
        mLabelColor = QColor(100, 100, 100);

        mCrossedPatternBursh.setStyle(Qt::BrushStyle::CrossPattern);
        mCrossedPatternBursh.setColor(QColor(211, 211, 211));
    }

    // Target Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHandle.setPen(pen);
        mTargetHandle.setBrush(QColor(255, 0, 0));
        mTargetHandle.setHoveredBrush(QColor(255, 255, 0));
        mTargetHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetHandle.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleTop.setPen(pen);
        mFovWidthHandleTop.setBrush(QColor(255, 128, 0));
        mFovWidthHandleTop.setHoveredBrush(QColor(255, 255, 0));
        mFovWidthHandleTop.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleTop.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleBottom.setPen(pen);
        mFovWidthHandleBottom.setBrush(QColor(255, 128, 0));
        mFovWidthHandleBottom.setHoveredBrush(QColor(255, 255, 0));
        mFovWidthHandleBottom.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleBottom.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mCameraHandle.setPen(pen);
        mCameraHandle.setBrush(QColor(255, 128, 0));
        mCameraHandle.setSize(10, 10);
    }

    setMouseTracking(true);
}

void TopViewWidget::setParameters(Dori::Core::Controller::TopViewWidgetParamaters *newParameters)
{
    mParameters = newParameters;
}

void TopViewWidget::refresh()
{
    updateHandles();
    update();
}

QPointF TopViewWidget::mapFrom3d(Eigen::Vector3f vector)
{
    float x = mOrigin.x() + vector.x() * mMeterToPixelRatio;
    float y = mOrigin.y() - vector.y() * mMeterToPixelRatio;
    return QPointF(x, y);
}

Eigen::Vector3f TopViewWidget::mapFrom2d(QPointF point)
{
    Eigen::Vector3f vector;

    vector[0] = (point.x() - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = (mOrigin.y() - point.y()) / mMeterToPixelRatio;
    vector[2] = 0;
    return vector;
}

void TopViewWidget::updateHandles()
{
    mTargetHandle.setCenter(mapFrom3d(Eigen::Vector3f(mParameters->targetDistance, 0, 0)));
    mFovWidthHandleTop.setCenter(mParameters->target[0]);
    mFovWidthHandleBottom.setCenter(mParameters->target[3]);
    mCameraHandle.setCenter(mOrigin.x(), mOrigin.y());
}

void TopViewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Draw crossed pattern
    painter.fillRect(0, 0, width(), height(), mCrossedPatternBursh);

    // Draw ground and frustum intersection
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.moveTo(mParameters->ground[0]);
    path.lineTo(mParameters->ground[1]);
    path.lineTo(mParameters->ground[2]);
    path.lineTo(mParameters->ground[3]);
    path.closeSubpath();
    mSolidPen.setColor(QColor(128, 128, 128));
    mSolidPen.setWidth(1);
    mSolidPen.setCapStyle(Qt::FlatCap);
    painter.setPen(mSolidPen);
    painter.drawPath(path);

    // Draw frustum and target intersection
    mSolidPen.setColor(QColor(0, 102, 213));
    mSolidPen.setWidth(1);
    mSolidPen.setCapStyle(Qt::FlatCap);
    painter.setPen(mSolidPen);
    painter.drawLine(mOrigin, mParameters->ground[1]);
    painter.drawLine(mOrigin, mParameters->ground[2]);
    painter.drawLine(mOrigin, mParameters->target[0]);
    painter.drawLine(mOrigin, mParameters->target[3]);

    mDashedPen.setColor(QColor(128, 128, 128));
    painter.setPen(mDashedPen);
    painter.drawLine(mParameters->target[0], mParameters->ground[0]);
    painter.drawLine(mParameters->target[3], mParameters->ground[3]);

    painter.setRenderHint(QPainter::Antialiasing, false);

    mSolidPen.setColor(QColor(0, 128, 0));
    mSolidPen.setWidth(3);
    mSolidPen.setCapStyle(Qt::FlatCap);
    painter.setPen(mSolidPen);
    painter.drawLine(mFovWidthHandleBottom.getCenter(1, 0), mFovWidthHandleTop.getCenter(1, 0));

    mCameraHandle.draw(this);
    mTargetHandle.draw(this);
    mFovWidthHandleTop.draw(this);
    mFovWidthHandleBottom.draw(this);
}

void TopViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (mTargetHandle.contains(event->pos())) {
        mTargetHandle.setPressed(true);
    } else if (mFovWidthHandleTop.contains(event->pos())) {
        mFovWidthHandleTop.setPressed(true);
    } else if (mFovWidthHandleBottom.contains(event->pos())) {
        mFovWidthHandleBottom.setPressed(true);
    } else {
        mMousePressedOnCanvas = true;
    }

    mOldMousePosition = event->pos();
    update();
}

void TopViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    mTargetHandle.setHovered(mTargetHandle.contains(event->pos()));
    mFovWidthHandleTop.setHovered(mFovWidthHandleTop.contains(event->pos()));
    mFovWidthHandleBottom.setHovered(mFovWidthHandleBottom.contains(event->pos()));

    bool isDirty = false;

    if (mTargetHandle.pressed()) {
        isDirty = true;
        mParameters->targetDistance += (event->pos() - mOldMousePosition).x() / mMeterToPixelRatio;
    }

    update();

    if (!isDirty && mMousePressedOnCanvas)
        emit pan((event->pos() - mOldMousePosition).x(), (event->pos() - mOldMousePosition).y());

    if (isDirty)
        emit dirty();

    mOldMousePosition = event->pos();
}

void TopViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHandle.setPressed(false);
    mFovWidthHandleTop.setPressed(false);
    mFovWidthHandleBottom.setPressed(false);
    mMousePressedOnCanvas = false;

    mOldMousePosition = event->pos();
    update();
}

void TopViewWidget::wheelEvent(QWheelEvent *event)
{
    emit zoom(event->angleDelta().y());
}

void TopViewWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;
}

void TopViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}
