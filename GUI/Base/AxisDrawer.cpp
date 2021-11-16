#include "AxisDrawer.h"

AxisDrawer::AxisDrawer(QPaintDevice *device)
    : mPaintDevice(device)
{}

void AxisDrawer::draw(const Parameters &parameters)
{
    QPainter painter(mPaintDevice);
    painter.save();

    int width = mPaintDevice->width();
    int height = mPaintDevice->height();
    const QPointF &origin = parameters.origin;

    // Draw horizontal axis (x-axis)
    {
        const QPen &pen = parameters.horizontal.pen;
        const float &tickmarkValueStep = parameters.horizontal.tickmarkValueStep;
        const int &minorTickmarkCount = parameters.horizontal.minorTickmarkCount;
        const TickmarkStyle &tickmarkStyle = parameters.horizontal.tickmarkStyle;
        const TickmarkStyle &minorTickmarkStyle = parameters.horizontal.minorTickmarkStyle;
        const float &valueToPixelRatio = parameters.valueToPixelRatio;
        const QFont &labelFont = parameters.horizontal.labelStyle.font;
        const QPen &labelPen = parameters.horizontal.labelStyle.pen;

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
                    painter.fillRect(x, y, w, h, parameters.horizontal.minorTickmarkStyle.brush);
                }

                // Don't draw tickmarks in the neighbourhood of the origin
                if (abs(X - origin.x()) < W)
                    continue;

                // Draw tickmark
                painter.fillRect(X, Y, W, H, parameters.horizontal.tickmarkStyle.brush);

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
        const QPen &pen = parameters.vertical.pen;
        const float &tickmarkValueStep = parameters.vertical.tickmarkValueStep;
        const int &minorTickmarkCount = parameters.vertical.minorTickmarkCount;
        const TickmarkStyle &tickmarkStyle = parameters.vertical.tickmarkStyle;
        const TickmarkStyle &minorTickmarkStyle = parameters.vertical.minorTickmarkStyle;
        const float &valueToPixelRatio = parameters.valueToPixelRatio;
        const QFont &labelFont = parameters.vertical.labelStyle.font;
        const QPen &labelPen = parameters.vertical.labelStyle.pen;

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
                    painter.fillRect(x, y, w, h, parameters.vertical.minorTickmarkStyle.brush);
                }

                // Don't draw tickmarks in the neighbourhood of the origin
                if (abs(Y - origin.y()) < H)
                    continue;

                // Draw tickmark
                painter.fillRect(X, Y, W, H, parameters.vertical.tickmarkStyle.brush);

                // Don't draw labels in the neighbourhood of the origin
                if (abs(Y - origin.y()) < 20)
                    continue;

                float value = (origin.y() - Y) / valueToPixelRatio;
                QString label = QString::number(value, 'f', 0);

                QRectF boundingBox(X - 2000,
                                   Y - 0.5 * labelFont.pixelSize(),
                                   1995,
                                   labelFont.pixelSize());
                painter.drawText(boundingBox, Qt::AlignRight, label);
            }
        }
    }

    painter.restore();
}
