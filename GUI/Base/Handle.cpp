#include "Handle.h"

#include <QMouseEvent>
#include <QPainter>

Handle::Handle(QWidget *widget)
    : mWidget(widget)
    , mPressed(false)
    , mHovered(false)
{}

bool Handle::contains(float x, float y)
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
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(mPen);

    if (mPressed)
        painter.setBrush(mPressedBrush);
    else if (mHovered)
        painter.setBrush(mHoveredBrush);
    else
        painter.setBrush(mBrush);

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

void Handle::setCenter(const QPointF &center)
{
    setCenter(center.x(), center.y());
}

void Handle::setSize(float w, float h)
{
    mRectangle = QRect(mRectangle.x(), mRectangle.y(), w, h);
}

QPointF Handle::getCenter()
{
    return mRectangle.center();
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

void Handle::setCenter(float x, float y)
{
    mRectangle = QRect(x - 0.5 * mRectangle.width(), y - 0.5 * mRectangle.height(), mRectangle.width(), mRectangle.height());
}
