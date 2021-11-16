#include "Handle.h"

#include <QPainter>

Handle::Handle() {}

bool Handle::contains(int x, int y)
{
    return mRectangle.contains(x, y);
}

bool Handle::contains(QPointF point)
{
    return mRectangle.contains(point);
}

void Handle::draw(QPaintDevice *device)
{
    QPainter painter(device);
    painter.save();

    painter.setPen(mPen);
    painter.drawRect(mRectangle);

    if (mPressed) {
        painter.fillRect(mRectangle, mPressedBursh);
    } else if (mHovered) {
        painter.fillRect(mRectangle, mHoveredBrush);
    } else {
        painter.fillRect(mRectangle, mBrush);
    }

    painter.restore();
}

bool Handle::pressed() const
{
    return mPressed;
}

void Handle::setPressed(bool newPressed)
{
    mPressed = newPressed;
}

const QPen &Handle::pen() const
{
    return mPen;
}

void Handle::setPen(const QPen &newPen)
{
    mPen = newPen;
}

const QBrush &Handle::brush() const
{
    return mBrush;
}

void Handle::setBrush(const QBrush &newBrush)
{
    mBrush = newBrush;
}

const QRectF &Handle::rectangle() const
{
    return mRectangle;
}

void Handle::setRectangle(const QRectF &newRectangle)
{
    mRectangle = newRectangle;
}

const QBrush &Handle::pressedBursh() const
{
    return mPressedBursh;
}

void Handle::setPressedBursh(const QBrush &newPressedBursh)
{
    mPressedBursh = newPressedBursh;
}

const QBrush &Handle::hoveredBrush() const
{
    return mHoveredBrush;
}

void Handle::setHoveredBrush(const QBrush &newHoveredBrush)
{
    mHoveredBrush = newHoveredBrush;
}
