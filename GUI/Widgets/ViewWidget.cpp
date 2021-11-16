#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
    , mAxisDrawer(this)
{}

const AxisDrawer::AxisParameters &ViewWidget::horizontalAxisParameters() const
{
    return mHorizontalAxisParameters;
}

void ViewWidget::setHorizontalAxisParameters(const AxisDrawer::AxisParameters &newHorizontalAxisParameters)
{
    mHorizontalAxisParameters = newHorizontalAxisParameters;
}

const AxisDrawer::AxisParameters &ViewWidget::verticalAxisParameters() const
{
    return mVerticalAxisParameters;
}

void ViewWidget::setVerticalAxisParameters(const AxisDrawer::AxisParameters &newVerticalAxisParameters)
{
    mVerticalAxisParameters = newVerticalAxisParameters;
}

QPointF ViewWidget::origin() const
{
    return mOrigin;
}

void ViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}

float ViewWidget::valueToPixelRatio() const
{
    return mValueToPixelRatio;
}

void ViewWidget::setValueToPixelRatio(float newValueToPixelRatio)
{
    mValueToPixelRatio = newValueToPixelRatio;
}
