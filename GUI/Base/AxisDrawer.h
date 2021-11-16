#ifndef AXISDRAWER_H
#define AXISDRAWER_H

#include <QPainter>

class AxisDrawer
{
public:
    AxisDrawer(QPaintDevice *device);

    struct TickmarkStyle
    {
        QSizeF size;
        QBrush brush;
    };

    struct LabelStyle
    {
        QFont font;
        QPen pen;
    };

    struct AxisParameters
    {
        TickmarkStyle tickmarkStyle;
        TickmarkStyle minorTickmarkStyle;
        LabelStyle labelStyle;
        QPen pen;
        int minorTickmarkCount;
        float tickmarkValueStep;
    };

    struct Parameters
    {
        AxisParameters vertical;
        AxisParameters horizontal;
        QPoint origin;
        float valueToPixelRatio;
    };

    void draw(const Parameters &parameters);

    QPaintDevice *mPaintDevice;
};

#endif // AXISDRAWER_H
