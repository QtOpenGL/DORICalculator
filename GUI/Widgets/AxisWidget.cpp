#include "AxisWidget.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

AxisWidget::AxisWidget(QWidget *parent)
    : QWidget(parent)
{
    mMinorTickmarkCount = 1;
    mTickmarkPixelStep = 50;

    mAxisPen = QPen(QColor(0, 0, 0));
    mAxisPen.setWidthF(1);

    mLabelFont = QFont("Arial");
    mLabelFont.setPixelSize(9);

    mTickmarkSize = QSizeF(1.5, 6);
    mMinorTickmarkSize = QSizeF(1, 4);

    mLabelColor = QColor(0, 0, 0);
    mTickmarkColor = QColor(100, 100, 100);
    mMinorTickmarkColor = QColor(0, 0, 0);
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

            QString label;
            if (mMeterToPixelRatio > 16)
                label = QString::number(value, 'f', 2);
            else
                label = QString::number(value, 'f', 0);

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

            QString label;
            if (mMeterToPixelRatio > 16)
                label = QString::number(value, 'f', 2);
            else
                label = QString::number(value, 'f', 0);

            QRectF boundingBox(mOrigin.x() - 105, tickmark.y() - 0.5 * mLabelFont.pixelSize(), 100, mLabelFont.pixelSize());
            painter.drawText(boundingBox, Qt::AlignRight, label);
        }
    }
}

void AxisWidget::setTickmarkPixelStep(int newTickmarkPixelStep)
{
    mTickmarkPixelStep = newTickmarkPixelStep;
}

void AxisWidget::setMinorTickmarkCount(int newMinorTickmarkCount)
{
    mMinorTickmarkCount = newMinorTickmarkCount;
}

void AxisWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;
}

void AxisWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}
