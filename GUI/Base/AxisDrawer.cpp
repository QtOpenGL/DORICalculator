#include "AxisDrawer.h"

#include <GUI/Widgets/ViewWidget.h>

AxisDrawer::AxisDrawer(ViewWidget *widget)
    : mWidget(widget)
{}

void AxisDrawer::draw()
{
    QPainter painter(mWidget);
    painter.save();

    int width = mWidget->width();
    int height = mWidget->height();
    const QPointF &origin = mWidget->origin();
    const float &valueToPixelRatio = mWidget->valueToPixelRatio();

    // Draw horizontal axis (x-axis)
    {
        const QPen &pen = mWidget->horizontalAxisParameters().pen;
        const float &tickmarkValueStep = mWidget->horizontalAxisParameters().tickmarkValueStep;
        const int &minorTickmarkCount = mWidget->horizontalAxisParameters().minorTickmarkCount;
        const TickmarkStyle &tickmarkStyle = mWidget->horizontalAxisParameters().tickmarkStyle;
        const TickmarkStyle &minorTickmarkStyle = mWidget->horizontalAxisParameters().minorTickmarkStyle;
        const QFont &labelFont = mWidget->horizontalAxisParameters().labelStyle.font;
        const QPen &labelPen = mWidget->horizontalAxisParameters().labelStyle.pen;

        // Axis line
        {
            QLine line = QLine(0, origin.y(), width, origin.y());
            painter.setPen(pen);
            painter.drawLine(line);
        }

        // Tickmarks and labels
        {
            // Major tickmark parameters
            int Y = origin.y() - 0.5 * tickmarkStyle.size.height();
            int W = tickmarkStyle.size.width();
            int H = tickmarkStyle.size.height();

            // Minor tickmark parameters
            int y = origin.y() - 0.5 * minorTickmarkStyle.size.height();
            int w = minorTickmarkStyle.size.width();
            int h = minorTickmarkStyle.size.height();

            int tickmarkPixelStep = tickmarkValueStep * valueToPixelRatio;

            painter.setFont(labelFont);
            painter.setPen(labelPen);

            // Main loop for tickmarks, labels, and minor tickmarks
            for (int X = 0; X < width; X += tickmarkPixelStep) {
                // Minor tickmarks
                for (int i = 1; i <= minorTickmarkCount; i++) {
                    int x = X + i * (tickmarkPixelStep / (minorTickmarkCount + 1));

                    // Don't draw minor tickmarks in the neighbourhood of the origin
                    if (abs(x - origin.x()) < w)
                        continue;

                    // Draw minor tickmark
                    painter.fillRect(x, y, w, h, mWidget->horizontalAxisParameters().minorTickmarkStyle.brush);
                }

                // Don't draw tickmarks in the neighbourhood of the origin
                if (abs(X - origin.x()) < W)
                    continue;

                // Draw tickmark
                painter.fillRect(X, Y, W, H, mWidget->horizontalAxisParameters().tickmarkStyle.brush);

                // Don't draw labels in the neighbourhood of the origin
                if (abs(X - origin.x()) < 20)
                    continue;

                // Draw label
                float value = (X - origin.x()) / valueToPixelRatio;
                QString label = QString::number(value, 'f', 0);
                QRectF boundingBox(X - 1000, Y + 10, 2000, labelFont.pixelSize());
                painter.drawText(boundingBox, Qt::AlignCenter, label);
            }
        }
    }

    // Draw vertical axis (y-axis)
    {
        const QPen &pen = mWidget->verticalAxisParameters().pen;
        const float &tickmarkValueStep = mWidget->verticalAxisParameters().tickmarkValueStep;
        const int &minorTickmarkCount = mWidget->verticalAxisParameters().minorTickmarkCount;
        const TickmarkStyle &tickmarkStyle = mWidget->verticalAxisParameters().tickmarkStyle;
        const TickmarkStyle &minorTickmarkStyle = mWidget->verticalAxisParameters().minorTickmarkStyle;
        const QFont &labelFont = mWidget->verticalAxisParameters().labelStyle.font;
        const QPen &labelPen = mWidget->verticalAxisParameters().labelStyle.pen;

        // Axis line
        {
            QLine line = QLine(origin.x(), 0, origin.x(), height);
            painter.setPen(pen);
            painter.drawLine(line);
        }

        // Tickmarks and labels
        {
            // Major tickmark parameters
            int X = origin.x() - 0.5 * tickmarkStyle.size.width();
            int W = tickmarkStyle.size.width();
            int H = tickmarkStyle.size.height();

            // Minor tickmark parameters
            int x = origin.x() - 0.5 * minorTickmarkStyle.size.width();
            int w = minorTickmarkStyle.size.width();
            int h = minorTickmarkStyle.size.height();

            int tickmarkPixelStep = tickmarkValueStep * valueToPixelRatio;

            painter.setFont(labelFont);
            painter.setPen(labelPen);

            // Main loop for tickmarks, labels, and minor tickmarks
            for (int Y = 0; Y < height; Y += tickmarkPixelStep) {
                // Minor tickmarks
                for (int i = 1; i <= minorTickmarkCount; i++) {
                    int y = Y + i * (tickmarkPixelStep / (minorTickmarkCount + 1));
                    // Don't draw minor tickmarks in the neighbourhood of the origin
                    if (abs(y - origin.y()) < h)
                        continue;
                    painter.fillRect(x, y, w, h, mWidget->verticalAxisParameters().minorTickmarkStyle.brush);
                }

                // Don't draw tickmarks in the neighbourhood of the origin
                if (abs(Y - origin.y()) < H)
                    continue;

                // Draw tickmark
                painter.fillRect(X, Y, W, H, mWidget->verticalAxisParameters().tickmarkStyle.brush);

                // Don't draw labels in the neighbourhood of the origin
                if (abs(Y - origin.y()) < 20)
                    continue;

                float value = (origin.y() - Y) / valueToPixelRatio;
                QString label = QString::number(value, 'f', 0);

                QRectF boundingBox(X - 2000, Y - 0.5 * labelFont.pixelSize(), 1995, labelFont.pixelSize());
                painter.drawText(boundingBox, Qt::AlignRight, label);
            }
        }
    }

    painter.restore();
}
