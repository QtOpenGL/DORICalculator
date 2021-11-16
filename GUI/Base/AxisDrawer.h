#ifndef AXISDRAWER_H
#define AXISDRAWER_H

#include <QPainter>

class ViewWidget;

class AxisDrawer
{
public:
    AxisDrawer(ViewWidget *widget);

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

    void draw();

    ViewWidget *mWidget;
};

#endif // AXISDRAWER_H
