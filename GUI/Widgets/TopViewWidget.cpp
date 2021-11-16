#include "TopViewWidget.h"

#include <QPainter>

TopViewWidget::TopViewWidget(QWidget *parent)
    : QWidget(parent)
{}

void TopViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::CrossPattern);
    brush.setColor(QColor(200, 200, 200));

    painter.fillRect(0, 0, width(), height(), brush);
}
