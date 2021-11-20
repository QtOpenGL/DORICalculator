#include "SideViewWidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent)
    : QWidget(parent)
    , mTargetHeightHandle(this)
    , mTargetDistanceHandle(this)
    , mCameraHeightHandle(this)
{
    setMouseTracking(true);
}

void SideViewWidget::init()
{
    // General GUI
    {
        QVector<qreal> dashes;
        dashes << 2 << 2;
        mDashedPen.setDashPattern(dashes);
        mDashedPen.setStyle(Qt::DashLine);
    }

    // Axis parameters
    {
        mLabelFont = QFont("Arial");
        mLabelFont.setPixelSize(9);
        mLabelColor = QColor(0, 0, 0);
        mTickmarkColor = QColor(100, 100, 100);
        mTickmarkSize = QSizeF(1.5, 6);
        mMinorTickmarkColor = QColor(0, 0, 0);
        mMinorTickmarkSize = QSizeF(1, 4);
        mAxisPen = QPen(QColor(0, 0, 0));
        mAxisPen.setWidthF(1);
    }

    // Target Height Handle
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

    // Target Distance Handle
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

    // Camera Height Handler
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

    refresh();
}

void SideViewWidget::refresh()
{
    updateHandles();
    update();
}

QPointF SideViewWidget::mapFromCartesian(Eigen::Vector3f vector)
{
    return QPointF(mParameters->origin.x() + vector.x() * mParameters->meterToPixelRatio, mParameters->origin.y() - vector.z() * mParameters->meterToPixelRatio);
}

Eigen::Vector3f SideViewWidget::mapFromGui(QPointF point)
{
    Eigen::Vector3f vector;

    vector[0] = (point.x() - mParameters->origin.x()) / mParameters->meterToPixelRatio;
    vector[1] = 0;
    vector[2] = (mParameters->origin.y() - point.y()) / mParameters->meterToPixelRatio;
    return vector;
}

void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawAxis(Horizontal);
    drawAxis(Vertical);

    painter.save();

    // Bisector
    mDashedPen.setColor(QColor(0, 102, 213));
    painter.setPen(mDashedPen);
    painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->intersections[0]);

    // V1
    painter.setPen(QColor(0, 102, 213));
    painter.drawLine(mCameraHeightHandle.getCenter(), mTargetHeightHandle.getCenter());

    mDashedPen.setColor(QColor(128, 128, 128));
    painter.setPen(mDashedPen);
    painter.drawLine(mTargetHeightHandle.getCenter(), mParameters->intersections[1]);

    // V2
    painter.setPen(QColor(0, 102, 213));
    painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->intersections[2]);

    painter.restore();

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
        mParameters->target.setY(mParameters->target.y() + (event->pos() - mOldMousePosition).y());
    }

    if (mTargetDistanceHandle.pressed()) {
        isDirty = true;
        mParameters->target.setX(mParameters->target.x() + (event->pos() - mOldMousePosition).x());
    }

    if (mCameraHeightHandle.pressed()) {
        isDirty = true;
        mParameters->camera.setY(mParameters->camera.y() + (event->pos() - mOldMousePosition).y());
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

void SideViewWidget::wheelEvent(QWheelEvent *event)
{
    emit zoom(event->angleDelta().y());
}

void SideViewWidget::setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters)
{
    mParameters = newParameters;
}

void SideViewWidget::updateHandles()
{
    mTargetHeightHandle.setCenter(mParameters->target.x(), mParameters->target.y());
    mTargetDistanceHandle.setCenter(mParameters->target.x(), mParameters->origin.y());
    mCameraHeightHandle.setCenter(mParameters->camera.x(), mParameters->camera.y());
}

void SideViewWidget::drawAxis(Axis axis)
{
    QPainter painter(this);
    painter.save();

    QLine line = axis == Horizontal ? QLine(0, mParameters->origin.y(), width(), mParameters->origin.y()) : QLine(mParameters->origin.x(), 0, mParameters->origin.x(), height());

    painter.setPen(mAxisPen);
    painter.drawLine(line);

    QRectF tickmark = axis == Horizontal ? QRectF(0, 0, mTickmarkSize.width(), mTickmarkSize.height()) : QRectF(0, 0, mTickmarkSize.height(), mTickmarkSize.width());

    QRectF minorTickmark = axis == Horizontal ? QRectF(0, 0, mMinorTickmarkSize.width(), mMinorTickmarkSize.height())
                                              : QRectF(0, 0, mMinorTickmarkSize.height(), mMinorTickmarkSize.width());

    painter.setFont(mLabelFont);
    painter.setPen(mLabelColor);

    const QRectF originBoundingBox(mParameters->origin.x() - 8, mParameters->origin.y() - 8, 16, 16);

    int start = 0;
    int end = 0;

    if (axis == Horizontal) {
        start = mParameters->origin.x() - mParameters->tickmarkPixelStep * floor(mParameters->origin.x() / mParameters->tickmarkPixelStep);
        end = width();
    } else {
        start = mParameters->origin.y() - mParameters->tickmarkPixelStep * floor(mParameters->origin.y() / mParameters->tickmarkPixelStep);
        end = height();
    }

    // Main loop for tickmarks, labels, and minor tickmarks
    for (int var = start; var < end; var += mParameters->tickmarkPixelStep) {
        // Minor tickmarks
        for (int i = 1; i <= mParameters->minorTickmarkCount; i++) {
            float step = (float) (mParameters->tickmarkPixelStep / (mParameters->minorTickmarkCount + 1));
            if (axis == Horizontal)
                minorTickmark.moveCenter(QPointF(var + i * step, mParameters->origin.y()));
            else
                minorTickmark.moveCenter(QPointF(mParameters->origin.x(), var + i * step));

            if (originBoundingBox.intersects(minorTickmark))
                continue;

            painter.fillRect(minorTickmark, mMinorTickmarkColor);
        }
        if (axis == Horizontal)
            tickmark.moveCenter(QPointF(var, mParameters->origin.y()));
        else
            tickmark.moveCenter(QPointF(mParameters->origin.x(), var));

        if (originBoundingBox.intersects(tickmark))
            continue;

        painter.fillRect(tickmark, mTickmarkColor);

        if (axis == Horizontal) {
            tickmark.moveCenter(QPointF(var, mParameters->origin.y()));
            float value = (var - mParameters->origin.x()) / mParameters->meterToPixelRatio;

            QString label;
            if (mParameters->meterToPixelRatio > 16)
                label = QString::number(value, 'f', 2);
            else
                label = QString::number(value, 'f', 0);

            QRectF boundingBox(var - 50, tickmark.y() + 10, 100, mLabelFont.pixelSize());
            painter.drawText(boundingBox, Qt::AlignCenter, label);
        }

        else {
            tickmark.moveCenter(QPointF(mParameters->origin.x(), var));
            float value = (mParameters->origin.y() - var) / mParameters->meterToPixelRatio;

            QString label;
            if (mParameters->meterToPixelRatio > 16)
                label = QString::number(value, 'f', 2);
            else
                label = QString::number(value, 'f', 0);

            QRectF boundingBox(tickmark.x() - 100, var - 0.5 * mLabelFont.pixelSize(), 95, mLabelFont.pixelSize());
            painter.drawText(boundingBox, Qt::AlignRight, label);
        }
    }

    painter.restore();
}
