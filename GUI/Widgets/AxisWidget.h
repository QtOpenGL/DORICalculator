#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <QPen>
#include <QWidget>

class AxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AxisWidget(QWidget *parent = nullptr);
    void refresh();

    void setOrigin(QPointF newOrigin);
    void setMeterToPixelRatio(float newMeterToPixelRatio);
    void setMinorTickmarkCount(int newMinorTickmarkCount);
    int findSuitableTickmarkPixelStep(float meterToPixelRatio);

private:
    void paintEvent(QPaintEvent *) override;

    QPointF mOrigin;
    float mMeterToPixelRatio;

    int mMinorTickmarkCount;
    int mTickmarkPixelStep;

    QPen mAxisPen;
    QFont mLabelFont;

    QSizeF mTickmarkSize;
    QSizeF mMinorTickmarkSize;

    QColor mTickmarkColor;
    QColor mMinorTickmarkColor;
    QColor mLabelColor;
};

#endif // AXISWIDGET_H
