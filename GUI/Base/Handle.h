#ifndef HANDLE_H
#define HANDLE_H

#include <QPen>

#include <GUI/Widgets/ViewWidget.h>

class Handle
{
public:
    Handle(ViewWidget *widget);

    bool contains(int x, int y);
    bool contains(QPoint point);

    void draw();

    bool pressed() const;
    void setPressed(bool newPressed);

    const QPen &pen() const;
    void setPen(const QPen &newPen);

    const QBrush &brush() const;
    void setBrush(const QBrush &newBrush);

    const QRect &rectangle() const;
    void setRectangle(const QRect &newRectangle);
    void setCenter(int x, int y);
    void setSize(int w, int h);
    void setCenterX(int x);
    void setCenterY(int y);
    void translate(int dx, int dy);

    const QBrush &hoveredBrush() const;
    void setHoveredBrush(const QBrush &newHoveredBrush);

    bool hovered() const;
    void setHovered(bool newHovered);

    const QBrush &pressedBrush() const;
    void setPressedBrush(const QBrush &newPressedBrush);

private:
    ViewWidget *mWidget;

    QRect mRectangle;
    QBrush mBrush, mPressedBrush, mHoveredBrush;
    QPen mPen;

    bool mPressed;
    bool mHovered;
};

#endif // HANDLE_H
