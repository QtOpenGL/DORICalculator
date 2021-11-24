#include "SideViewWidget.h"

#include <Core/Constants.h>
#include <Core/Enums.h>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStackedLayout>
#include <QtMath>

SideViewWidget::SideViewWidget(QWidget *parent)
    : QWidget(parent)
    , mMousePressedOnCanvas(false)
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

        mLabelColor = QColor(0, 0, 0);
    }

    // Target Height Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHeightHandle.setPen(pen);
        mTargetHeightHandle.setBrush(QColor(255, 0, 0));
        mTargetHeightHandle.setHoveredBrush(QColor(255, 255, 255));
        mTargetHeightHandle.setPressedBrush(QColor(0, 225, 0));
        mTargetHeightHandle.setSize(10, 10);
    }

    // Target Distance Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetDistanceHandle.setPen(pen);
        mTargetDistanceHandle.setBrush(QColor(255, 128, 0));
        mTargetDistanceHandle.setHoveredBrush(QColor(255, 255, 255));
        mTargetDistanceHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetDistanceHandle.setSize(10, 10);
    }

    // Camera Height Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mCameraHeightHandle.setPen(pen);
        mCameraHeightHandle.setBrush(QColor(63, 150, 157));
        mCameraHeightHandle.setHoveredBrush(QColor(255, 255, 255));
        mCameraHeightHandle.setPressedBrush(QColor(0, 255, 0));
        mCameraHeightHandle.setSize(10, 10);
    }

    // Lower Boundary Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mLowerBoundaryHandle.setPen(pen);
        mLowerBoundaryHandle.setBrush(QColor(255, 0, 0));
        mLowerBoundaryHandle.setHoveredBrush(QColor(255, 255, 255));
        mLowerBoundaryHandle.setPressedBrush(QColor(0, 255, 0));
        mLowerBoundaryHandle.setSize(10, 10);
    }

    setMouseTracking(true);
}

void SideViewWidget::refresh()
{
    updateHandles();
    update();
}

QPointF SideViewWidget::mapFrom3d(float distance, float height)
{
    float x = mOrigin.x() + distance * mMeterToPixelRatio;
    float y = mOrigin.y() - height * mMeterToPixelRatio;
    return QPointF(x, y);
}

QPointF SideViewWidget::mapFrom3d(Eigen::Vector3f vector)
{
    return mapFrom3d(vector.x(), vector.z());
}

Eigen::Vector3f SideViewWidget::mapFrom2d(float x, float y)
{
    Eigen::Vector3f vector;

    vector[0] = (x - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = 0;
    vector[2] = (mOrigin.y() - y) / mMeterToPixelRatio;
    return vector;
}

Eigen::Vector3f SideViewWidget::mapFrom2d(QPointF point)
{
    Eigen::Vector3f vector;

    vector[0] = (point.x() - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = 0;
    vector[2] = (mOrigin.y() - point.y()) / mMeterToPixelRatio;
    return vector;
}

void SideViewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Regions
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        for (enum RegionNames name : {STRONG_IDENTIFICATION, IDENTIFICATION, RECOGNITION, OBSERVATION, DETECTION, MONITORING}) {
            if (mParameters->regions[name].visible) {
                QPainterPath path;
                path.addPolygon(mParameters->regions[name].region);

                QBrush brush;
                brush.setStyle(Qt::BrushStyle::SolidPattern);
                brush.setColor(REGION_COLORS[name]);

                painter.fillPath(path, brush);
            }
        }
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    // Draw target height line
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        QPen pen(QColor(0, 128, 0));
        pen.setWidth(3);
        pen.setCapStyle(Qt::FlatCap);
        painter.setPen(pen);
        painter.drawLine(mTargetDistanceHandle.getCenter(1, 0), mTargetHeightHandle.getCenter(1, 0));
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    // Draw target height label
    {
        painter.setPen(QColor(0, 128, 0));
        painter.setFont(mLabelFont);
        QPointF point = QPointF(mTargetHeightHandle.getCenter().x() + 8, (mTargetDistanceHandle.getCenter().y() + mTargetHeightHandle.getCenter().y() + mLabelFont.pixelSize()) / 2);
        painter.drawText(point, QString::number(mParameters->target.height, 'f', 1) + " m");
    }
    {
        // Opposite bisector and bisector
        mDashedPen.setColor(QColor(0, 102, 213));
        painter.setPen(mDashedPen);
        painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[0]);
        painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[1]);
    }

    {
        painter.setPen(QColor(0, 102, 213));
        painter.drawLine(mCameraHeightHandle.getCenter(), mTargetHeightHandle.getCenter());

        mDashedPen.setColor(QColor(128, 128, 128));
        painter.setPen(mDashedPen);
        painter.drawLine(mTargetHeightHandle.getCenter(), mParameters->points[2]);

        painter.setPen(QColor(0, 102, 213));
        painter.drawLine(mCameraHeightHandle.getCenter(), mParameters->points[3]);
    }

    // Draw tilt angle reference line
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        mDashedPen.setColor(QColor(128, 128, 128));
        painter.setPen(mDashedPen);
        painter.drawLine(mCameraHeightHandle.getCenter(), QPoint(0, mCameraHeightHandle.getCenter().y()));
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    // Tilt angle
    {
        QPainterPath path;
        path.moveTo(mCameraHeightHandle.getCenter().x(), mCameraHeightHandle.getCenter().y());
        path.arcTo(mCameraHeightHandle.getCenter().x() - 50, mCameraHeightHandle.getCenter().y() - 50, 100, 100, -180, -mParameters->camera.tiltAngle);
        path.closeSubpath();
        QBrush brush;
        brush.setStyle(Qt::BrushStyle::Dense6Pattern);
        brush.setColor(QColor(0, 102, 213));
        painter.fillPath(path, brush);
    }

    // Tilt angle label
    {
        painter.setFont(mLabelFont);
        painter.setPen(mLabelColor);
        QString label = QString::number(mParameters->camera.tiltAngle, 'f', 2) + " º";
        QRectF boundingBox;
        if (mParameters->camera.tiltAngle > 0)
            boundingBox = QRectF(mCameraHeightHandle.getCenter().x() - 75, mCameraHeightHandle.getCenter().y(), 50, 2 * mLabelFont.pixelSize());
        else
            boundingBox = QRectF(mCameraHeightHandle.getCenter().x() - 75, mCameraHeightHandle.getCenter().y() - 2 * mLabelFont.pixelSize(), 50, 2 * mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignCenter, label);
    }

    // Draw handles
    mCameraHeightHandle.draw(this);
    mTargetHeightHandle.draw(this);
    mTargetDistanceHandle.draw(this);
    mLowerBoundaryHandle.draw(this);
}

void SideViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (mLowerBoundaryHandle.contains(event->pos())) {
        mLowerBoundaryHandle.setPressed(true);
    } else if (mTargetHeightHandle.contains(event->pos())) {
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
    mLowerBoundaryHandle.setHovered(mLowerBoundaryHandle.contains(event->pos()));

    bool isDirty = false;

    if (mTargetHeightHandle.pressed()) {
        float newTargetHeight = mParameters->target.height - (event->pos() - mOldMousePosition).y() / mMeterToPixelRatio;

        if (newTargetHeight >= 0.1) {
            isDirty = true;
            mParameters->target.height = newTargetHeight;
        }
    }

    if (mTargetDistanceHandle.pressed()) {
        isDirty = true;
        mParameters->target.distance += (event->pos() - mOldMousePosition).x() / mMeterToPixelRatio;
    }

    if (mCameraHeightHandle.pressed()) {
        float newCameraHeight = mParameters->camera.height - (event->pos() - mOldMousePosition).y() / mMeterToPixelRatio;

        if (newCameraHeight >= 0.25) {
            isDirty = true;
            mParameters->camera.height = newCameraHeight;
        }
    }

    if (mLowerBoundaryHandle.pressed()) {
        float newLowerBoundaryHeight = mParameters->lowerBoundary.height - (event->pos() - mOldMousePosition).y() / mMeterToPixelRatio;

        if (newLowerBoundaryHeight > 0 || newLowerBoundaryHeight < mParameters->target.height) {
            isDirty = true;
            mParameters->lowerBoundary.height = newLowerBoundaryHeight;
        }
    }

    if (isDirty)
        emit dirty();
    else if (mMousePressedOnCanvas)
        emit pan((event->pos() - mOldMousePosition).x(), (event->pos() - mOldMousePosition).y());

    if (!isDirty) // dirty signal will call update anyway
        update();

    mOldMousePosition = event->pos();
}

void SideViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHeightHandle.setPressed(false);
    mTargetDistanceHandle.setPressed(false);
    mCameraHeightHandle.setPressed(false);
    mLowerBoundaryHandle.setPressed(false);
    mMousePressedOnCanvas = false;

    mOldMousePosition = event->pos();
    update();
}

void SideViewWidget::wheelEvent(QWheelEvent *event)
{
    emit zoom(event->angleDelta().y());
}

void SideViewWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;
}

void SideViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}

void SideViewWidget::setParameters(Controller::SideViewWidgetParameters *newParameters)
{
    mParameters = newParameters;
}

void SideViewWidget::updateHandles()
{
    mTargetHeightHandle.setCenter(mParameters->target.position);
    mTargetDistanceHandle.setCenter(mParameters->target.position.x(), mOrigin.y());
    mCameraHeightHandle.setCenter(mParameters->camera.position);
    mLowerBoundaryHandle.setCenter(mParameters->lowerBoundary.position);
}
