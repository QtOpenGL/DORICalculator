#include "SideViewWidget.h"

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
    // Axis parameters
    {
        mTickmarkPixelStep = 40;
        mMinorTickmarkCount = 4;
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

void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    drawHorizontalAxis();
    drawVerticalAxis();

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

void SideViewWidget::setMinorTickmarkCount(int newMinorTickmarkCount)
{
    mMinorTickmarkCount = newMinorTickmarkCount;
}

int SideViewWidget::tickmarkPixelStep() const
{
    return mTickmarkPixelStep;
}

void SideViewWidget::setTickmarkPixelStep(int newTickmarkPixelStep)
{
    mTickmarkPixelStep = newTickmarkPixelStep;
}

int SideViewWidget::minorTickmarkCount() const
{
    return mMinorTickmarkCount;
}

float SideViewWidget::valueToPixelRatio() const
{
    return mValueToPixelRatio;
}

void SideViewWidget::setValueToPixelRatio(float newValueToPixelRatio)
{
    mValueToPixelRatio = newValueToPixelRatio;
}

QPointF SideViewWidget::origin() const
{
    return mOrigin;
}

void SideViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}

void SideViewWidget::setParameters(Dori::Core::Controller::SideViewWidgetParameters *newParameters)
{
    mParameters = newParameters;
}

void SideViewWidget::updateHandles()
{
    mTargetHeightHandle.setCenter(mOrigin.x() + mParameters->targetDistance, mOrigin.y() - mParameters->targetHeight);
    mTargetDistanceHandle.setCenter(mOrigin.x() + mParameters->targetDistance, mOrigin.y());
    mCameraHeightHandle.setCenter(mOrigin.x(), mOrigin.y() - mParameters->cameraHeight);
}

void SideViewWidget::drawHorizontalAxis()
{
    QPainter painter(this);
    painter.save();

    QLine line = QLine(0, mOrigin.y(), width(), mOrigin.y());
    painter.setPen(mAxisPen);
    painter.drawLine(line);

    QRectF tickmark(0, 0, mTickmarkSize.width(), mTickmarkSize.height());
    QRectF minorTickmark(0, 0, mMinorTickmarkSize.width(), mMinorTickmarkSize.height());

    painter.setFont(mLabelFont);
    painter.setPen(mLabelColor);

    const QRectF originBoundingBox(mOrigin.x() - 5, mOrigin.y() - 5, 10, 10);
    const QRectF originBigBoundingBox(mOrigin.x() - 10, mOrigin.y() - 10, 20, 20);

    // Main loop for tickmarks, labels, and minor tickmarks
    for (int x = 0; x < width(); x += mTickmarkPixelStep) {
        // Minor tickmarks
        for (int i = 1; i <= mMinorTickmarkCount; i++) {
            float step = (float) (mTickmarkPixelStep / (mMinorTickmarkCount + 1));

            minorTickmark.moveCenter(QPointF(x + i * step, mOrigin.y()));

            if (originBoundingBox.intersects(minorTickmark))
                continue;

            painter.fillRect(minorTickmark, mMinorTickmarkColor);
        }

        tickmark.moveCenter(QPointF(x, origin().y()));

        if (originBoundingBox.intersects(tickmark))
            continue;

        painter.fillRect(tickmark, mTickmarkColor);

        if (originBigBoundingBox.intersects(tickmark))
            continue;

        float value = (x - mOrigin.x()) / mValueToPixelRatio;
        QString label = QString::number(value, 'f', 0);
        QRectF boundingBox(x - 50, tickmark.y() + 10, 100, mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignCenter, label);
    }

    painter.restore();
}

void SideViewWidget::drawVerticalAxis()
{
    QPainter painter(this);
    painter.save();

    QLine line = QLine(mOrigin.x(), 0, mOrigin.x(), height());
    painter.setPen(mAxisPen);
    painter.drawLine(line);

    QRectF tickmark(0, 0, mTickmarkSize.height(), mTickmarkSize.width());
    QRectF minorTickmark(0, 0, mMinorTickmarkSize.height(), mMinorTickmarkSize.width());

    painter.setFont(mLabelFont);
    painter.setPen(mLabelColor);

    const QRectF originBoundingBox(mOrigin.x() - 5, mOrigin.y() - 5, 10, 10);
    const QRectF originBigBoundingBox(mOrigin.x() - 10, mOrigin.y() - 10, 20, 20);

    // Main loop for tickmarks, labels, and minor tickmarks
    for (int y = 0; y < width(); y += mTickmarkPixelStep) {
        // Minor tickmarks
        for (int i = 1; i <= mMinorTickmarkCount; i++) {
            float step = (float) (mTickmarkPixelStep / (mMinorTickmarkCount + 1));
            minorTickmark.moveCenter(QPointF(mOrigin.x(), y + i * step));

            if (originBoundingBox.intersects(minorTickmark))
                continue;

            painter.fillRect(minorTickmark, mMinorTickmarkColor);
        }

        tickmark.moveCenter(QPointF(mOrigin.x(), y));

        if (originBoundingBox.intersects(tickmark))
            continue;

        painter.fillRect(tickmark, mTickmarkColor);

        if (originBigBoundingBox.intersects(tickmark))
            continue;

        float value = (mOrigin.y() - y) / mValueToPixelRatio;
        QString label = QString::number(value, 'f', 0);
        QRectF boundingBox(tickmark.x() - 100, y - 0.5 * mLabelFont.pixelSize(), 95, mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignRight, label);
    }

    painter.restore();
}
