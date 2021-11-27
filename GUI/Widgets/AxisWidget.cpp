#include "AxisWidget.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

AxisWidget::AxisWidget(QWidget *parent)
    : QWidget(parent)
{
    mMinorTickmarkCount = 0;
    mTickmarkPixelStep = 50;

    mAxisPen = QPen(QColor(0, 0, 0));
    mAxisPen.setWidthF(1);

    mLabelFont = QFont();
    mLabelFont.setPixelSize(10);

    mTickmarkSize = QSizeF(1, 5);
    mMinorTickmarkSize = QSizeF(1, 4);

    mLabelColor = QColor(0, 0, 0);
    mTickmarkColor = QColor(100, 100, 100);
    mMinorTickmarkColor = QColor(0, 0, 0);

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AxisWidget::refresh()
{
    update();
}

void AxisWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Horizontal axis (x-axis)
    {
        QLine line = QLine(0, mOrigin.y(), width(), mOrigin.y());

        painter.setPen(mAxisPen);
        painter.drawLine(line);

        QRectF tickmark = QRectF(0, 0, mTickmarkSize.width(), mTickmarkSize.height());
        QRectF minorTickmark = QRectF(0, 0, mMinorTickmarkSize.width(), mMinorTickmarkSize.height());

        painter.setFont(mLabelFont);
        painter.setPen(mLabelColor);

        const QRectF originBoundingBox(mOrigin.x() - 8, mOrigin.y() - 8, 16, 16);

        int start = mOrigin.x() - mTickmarkPixelStep * floor(mOrigin.x() / mTickmarkPixelStep);
        int end = width();
        float step = (float) (mTickmarkPixelStep / (mMinorTickmarkCount + 1));

        // Main loop for tickmarks, labels, and minor tickmarks
        for (int x = start; x < end; x += mTickmarkPixelStep) {
            // Minor tickmarks
            for (int i = 1; i <= mMinorTickmarkCount; i++) {
                minorTickmark.moveCenter(QPointF(x + i * step, mOrigin.y()));

                if (originBoundingBox.intersects(minorTickmark))
                    continue;

                painter.fillRect(minorTickmark, mMinorTickmarkColor);
            }

            tickmark.moveCenter(QPointF(x, mOrigin.y()));

            if (originBoundingBox.intersects(tickmark))
                continue;

            painter.fillRect(tickmark, mTickmarkColor);

            tickmark.moveCenter(QPointF(x, mOrigin.y()));

            float value = (x - mOrigin.x()) / mMeterToPixelRatio;
            float integral = (int) value;

            QString label;
            if (qFuzzyCompare(integral, value))
                label = QString::number(value, 'f', 0);
            else
                label = QString::number(value, 'f', 2);

            QRectF boundingBox(x - 50, tickmark.y() + 10, 100, mLabelFont.pixelSize());
            painter.drawText(boundingBox, Qt::AlignCenter, label);
        }
    }

    // Vertical axis (y-axis)
    {
        QLine line = QLine(mOrigin.x(), 0, mOrigin.x(), height());

        painter.setPen(mAxisPen);
        painter.drawLine(line);

        QRectF tickmark = QRectF(0, 0, mTickmarkSize.height(), mTickmarkSize.width());
        QRectF minorTickmark = QRectF(0, 0, mMinorTickmarkSize.height(), mMinorTickmarkSize.width());

        painter.setFont(mLabelFont);
        painter.setPen(mLabelColor);

        const QRectF originBoundingBox(mOrigin.x() - 8, mOrigin.y() - 8, 16, 16);

        int start = mOrigin.y() - mTickmarkPixelStep * floor(mOrigin.y() / mTickmarkPixelStep);
        int end = height();
        float step = (float) (mTickmarkPixelStep / (mMinorTickmarkCount + 1));

        // Main loop for tickmarks, labels, and minor tickmarks
        for (int y = start; y < end; y += mTickmarkPixelStep) {
            // Minor tickmarks
            for (int i = 1; i <= mMinorTickmarkCount; i++) {
                minorTickmark.moveCenter(QPointF(mOrigin.x(), y + i * step));

                if (originBoundingBox.intersects(minorTickmark))
                    continue;

                painter.fillRect(minorTickmark, mMinorTickmarkColor);
            }

            tickmark.moveCenter(QPointF(mOrigin.x(), y));

            if (originBoundingBox.intersects(tickmark))
                continue;

            painter.fillRect(tickmark, mTickmarkColor);

            tickmark.moveCenter(QPointF(mOrigin.x(), y));

            float value = (mOrigin.y() - y) / mMeterToPixelRatio;
            float integral = (int) value;

            QString label;
            if (qFuzzyCompare(integral, value))
                label = QString::number(value, 'f', 0);
            else
                label = QString::number(value, 'f', 2);

            QRectF boundingBox(mOrigin.x() - 105, tickmark.y() - 0.5 * mLabelFont.pixelSize(), 100, mLabelFont.pixelSize());
            painter.drawText(boundingBox, Qt::AlignRight, label);
        }
    }
}

void AxisWidget::setMinorTickmarkCount(int newMinorTickmarkCount)
{
    mMinorTickmarkCount = newMinorTickmarkCount;
}

int AxisWidget::findSuitableTickmarkPixelStep(float meterToPixelRatio)
{
    float tickmarkMeterStep = 36 / meterToPixelRatio;

    int powers = 0;

    while (tickmarkMeterStep > 1) {
        tickmarkMeterStep = tickmarkMeterStep / 10.0f;
        powers++;
    }

    if (tickmarkMeterStep <= 0.25)
        return round(0.25f * pow(10, powers) * meterToPixelRatio);
    else if (tickmarkMeterStep <= 0.5)
        return round(0.5f * pow(10, powers) * meterToPixelRatio);
    else if (tickmarkMeterStep <= 0.75)
        return round(0.75f * pow(10, powers) * meterToPixelRatio);
    else
        return round(pow(10, powers) * meterToPixelRatio);
}

void AxisWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;

    mTickmarkPixelStep = findSuitableTickmarkPixelStep(newMeterToPixelRatio);
}

void AxisWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}
