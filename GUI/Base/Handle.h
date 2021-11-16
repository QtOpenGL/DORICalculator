#ifndef HANDLE_H
#define HANDLE_H

#include <QPen>

class Handle
{
public:
    Handle();

    bool contains(int x, int y);
    bool contains(QPointF point);

    void draw(QPaintDevice *device);

    bool pressed() const;
    void setPressed(bool newPressed);

    const QPen &pen() const;
    void setPen(const QPen &newPen);

    const QBrush &brush() const;
    void setBrush(const QBrush &newBrush);

    const QRectF &rectangle() const;
    void setRectangle(const QRectF &newRectangle);

    const QBrush &pressedBursh() const;
    void setPressedBursh(const QBrush &newPressedBursh);

    const QBrush &hoveredBrush() const;
    void setHoveredBrush(const QBrush &newHoveredBrush);

private:
    QRectF mRectangle;
    QBrush mBrush, mPressedBursh, mHoveredBrush;
    QPen mPen;

    bool mPressed;
    bool mHovered;
};

#endif // HANDLE_H
