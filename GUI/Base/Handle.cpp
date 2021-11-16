#include "Handle.h"

#include <QMouseEvent>
#include <QPainter>

Handle::Handle(ViewWidget *widget)
    : mWidget(widget)
    , mPressed(false)
    , mHovered(false)
{}

bool Handle::contains(int x, int y)
{
    return mRectangle.contains(x, y);
}

bool Handle::contains(QPoint point)
{
    return mRectangle.contains(point);
}

void Handle::draw()
{
    QPainter painter(mWidget);
    painter.save();

    painter.setPen(mPen);

    if (mPressed) {
        painter.setBrush(mPressedBrush);
    } else if (mHovered) {
        painter.setBrush(mHoveredBrush);
    } else {
        painter.setBrush(mBrush);
    }
    painter.drawRect(mRectangle);

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

const QRect &Handle::rectangle() const
{
    return mRectangle;
}

void Handle::setRectangle(const QRect &newRectangle)
{
    mRectangle = newRectangle;
}

void Handle::setCenter(int x, int y)
{
    mRectangle = QRect(x - 0.5 * mRectangle.width(), y - 0.5 * mRectangle.height(), mRectangle.width(), mRectangle.height());
}

void Handle::setSize(int w, int h)
{
    mRectangle = QRect(mRectangle.x(), mRectangle.y(), w, h);
}

void Handle::setCenterX(int x)
{
    mRectangle = QRect(x - 0.5 * mRectangle.width(), mRectangle.y(), mRectangle.width(), mRectangle.height());
}

void Handle::setCenterY(int y)
{
    mRectangle = QRect(mRectangle.x(), y - 0.5 * mRectangle.height(), mRectangle.width(), mRectangle.height());
}

void Handle::translate(int dx, int dy)
{
    mRectangle = QRect(mRectangle.x() + dx, mRectangle.y() + dy, mRectangle.width(), mRectangle.height());
}

const QBrush &Handle::hoveredBrush() const
{
    return mHoveredBrush;
}

void Handle::setHoveredBrush(const QBrush &newHoveredBrush)
{
    mHoveredBrush = newHoveredBrush;
}

bool Handle::hovered() const
{
    return mHovered;
}

void Handle::setHovered(bool newHovered)
{
    mHovered = newHovered;
}

const QBrush &Handle::pressedBrush() const
{
    return mPressedBrush;
}

void Handle::setPressedBrush(const QBrush &newPressedBrush)
{
    mPressedBrush = newPressedBrush;
}
