#include "SideViewWidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

SideViewWidget::SideViewWidget(QWidget *parent)
    : QWidget(parent)
    , mTargetHeightHandle(this)
    , mTargetDistanceHandle(this)
    , mCameraHeightHandle(this)
    , mMousePressedOnCanvas(false)
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

        mSolidPen.setWidthF(1);

        mBigLabelFont = QFont("Arial");
        mBigLabelFont.setPixelSize(11);
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

QPointF SideViewWidget::mapFrom3d(Eigen::Vector3f vector)
{
    return QPointF(mParameters->origin.x() + vector.x() * mParameters->meterToPixelRatio, mParameters->origin.y() - vector.z() * mParameters->meterToPixelRatio);
}

Eigen::Vector3f SideViewWidget::mapFrom2d(QPointF point)
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

    drawAxis(Horizontal);
    drawAxis(Vertical);

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw Target Height Line
    QPen pen(QColor(0, 128, 0));
    pen.setWidth(3);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    painter.drawLine(mTargetDistanceHandle.getCenter() + QPointF(0.5, 0), mTargetHeightHandle.getCenter() + QPointF(0.5, 0));

    // Draw target height label
    painter.setPen(QColor(0, 128, 0));
    painter.setFont(mBigLabelFont);
    QPointF point = QPointF(mTargetHeightHandle.getCenter().x() + 8, (mTargetDistanceHandle.getCenter().y() + mTargetHeightHandle.getCenter().y() + mLabelFont.pixelSize()) / 2);
    painter.drawText(point, QString::number(mParameters->target.height, 'f', 1) + " m");

    // Opposite Bisector and Bisector
    mDashedPen.setColor(QColor(0, 102, 213));
    painter.setPen(mDashedPen);
    painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[Dori::Core::Logic::OPPOSITE_BISECTOR]);
    painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[Dori::Core::Logic::BISECTOR]);

    // V1
    painter.setPen(QColor(0, 102, 213));
    painter.drawLine(mCameraHeightHandle.getCenter(), mTargetHeightHandle.getCenter());

    mDashedPen.setColor(QColor(128, 128, 128));
    painter.setPen(mDashedPen);
    painter.drawLine(mTargetHeightHandle.getCenter(), mParameters->points[Dori::Core::Logic::V1]);

    // V2
    painter.setPen(QColor(0, 102, 213));
    painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[Dori::Core::Logic::V2]);

    // Draw tilt angle reference line
    mDashedPen.setColor(QColor(128, 128, 128));
    painter.setPen(mDashedPen);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawLine(mCameraHeightHandle.getCenter(), QPoint(0, mCameraHeightHandle.getCenter().y()));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.restore();

    // Tilt angle
    QPainterPath path;
    path.moveTo(mCameraHeightHandle.getCenter().x(), mCameraHeightHandle.getCenter().y());
    path.arcTo(mCameraHeightHandle.getCenter().x() - 50, mCameraHeightHandle.getCenter().y() - 50, 100, 100, -180, -mParameters->camera.tiltAngle);
    path.closeSubpath();
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::Dense6Pattern);
    brush.setColor(QColor(0, 102, 213));
    painter.fillPath(path, brush);

    // Tilt angle label
    painter.setFont(mBigLabelFont);
    painter.setPen(mLabelColor);
    QString label = QString::number(mParameters->camera.tiltAngle, 'f', 2) + " º";
    QRectF boundingBox;
    if (mParameters->camera.tiltAngle > 0)
        boundingBox = QRectF(mCameraHeightHandle.getCenter().x() - 75, mCameraHeightHandle.getCenter().y(), 50, 2 * mLabelFont.pixelSize());
    else
        boundingBox = QRectF(mCameraHeightHandle.getCenter().x() - 75, mCameraHeightHandle.getCenter().y() - 2 * mLabelFont.pixelSize(), 50, 2 * mLabelFont.pixelSize());
    painter.drawText(boundingBox, Qt::AlignCenter, label);

    // Draw handles
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
    } else {
        mMousePressedOnCanvas = true;
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
        float newTargetHeight = mParameters->target.height - (event->pos() - mOldMousePosition).y() / mParameters->meterToPixelRatio;

        if (newTargetHeight >= 0.1) {
            isDirty = true;
            mParameters->target.height = newTargetHeight;
        }
    }

    if (mTargetDistanceHandle.pressed()) {
        isDirty = true;
        mParameters->target.distance += (event->pos() - mOldMousePosition).x() / mParameters->meterToPixelRatio;
    }

    if (mCameraHeightHandle.pressed()) {
        float newCameraHeight = mParameters->camera.height - (event->pos() - mOldMousePosition).y() / mParameters->meterToPixelRatio;

        if (newCameraHeight >= 0.25) {
            isDirty = true;
            mParameters->camera.height = newCameraHeight;
        }
    }

    // Calling update() for each mouse move event may be expensive
    update();

    if (!isDirty && mMousePressedOnCanvas)
        emit pan((event->pos() - mOldMousePosition).x(), (event->pos() - mOldMousePosition).y());

    if (isDirty)
        emit dirty();

    mOldMousePosition = event->pos();
}

void SideViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setPressed(false);
    mTargetDistanceHandle.setPressed(false);
    mCameraHeightHandle.setPressed(false);
    mMousePressedOnCanvas = false;

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
    mTargetHeightHandle.setCenter(mParameters->target.position.x(), mParameters->target.position.y());
    mTargetDistanceHandle.setCenter(mParameters->target.position.x(), mParameters->origin.y());
    mCameraHeightHandle.setCenter(mParameters->camera.position.x(), mParameters->camera.position.y());
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
