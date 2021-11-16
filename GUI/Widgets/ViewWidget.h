#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QPointF>
#include <QWidget>

#include <GUI/Base/AxisDrawer.h>

class ViewWidget : public QWidget
{
public:
    explicit ViewWidget(QWidget *parent);

    const AxisDrawer::AxisParameters &horizontalAxisParameters() const;
    void setHorizontalAxisParameters(const AxisDrawer::AxisParameters &newHorizontalAxisParameters);

    const AxisDrawer::AxisParameters &verticalAxisParameters() const;
    void setVerticalAxisParameters(const AxisDrawer::AxisParameters &newVerticalAxisParameters);

    QPointF origin() const;
    void setOrigin(QPointF newOrigin);

    float valueToPixelRatio() const;
    void setValueToPixelRatio(float newValueToPixelRatio);

protected:
    AxisDrawer::AxisParameters mHorizontalAxisParameters;
    AxisDrawer::AxisParameters mVerticalAxisParameters;
    AxisDrawer mAxisDrawer;

    QPointF mOrigin;
    float mValueToPixelRatio;
};

#endif // VIEWWIDGET_H
