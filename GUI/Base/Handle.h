#ifndef HANDLE_H
#define HANDLE_H

#include <QPen>
#include <QWidget>

class Handle
{
public:
    Handle(QWidget *widget);

    bool contains(float x, float y);
    bool contains(QPoint point);

    void draw();

    bool pressed() const;
    void setPressed(bool newPressed);

    const QPen &pen() const;
    void setPen(const QPen &newPen);

    const QBrush &brush() const;
    void setBrush(const QBrush &newBrush);

    const QBrush &hoveredBrush() const;
    void setHoveredBrush(const QBrush &newHoveredBrush);

    bool hovered() const;
    void setHovered(bool newHovered);

    const QBrush &pressedBrush() const;
    void setPressedBrush(const QBrush &newPressedBrush);

    void setCenter(float x, float y);
    void setCenter(const QPointF &center);
    void setSize(float w, float h);
    QPointF getCenter();

private:
    QWidget *mWidget;

    QRect mRectangle;
    QBrush mBrush, mPressedBrush, mHoveredBrush;
    QPen mPen;

    bool mPressed;
    bool mHovered;
};

#endif // HANDLE_H
