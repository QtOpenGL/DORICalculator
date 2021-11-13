#include "SideViewWidget.h"

#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent) : QWidget(parent)
{

}


void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(QColor(180,180,180));

    painter.fillRect(0,0,width(), height(), brush);
}
