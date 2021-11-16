#include "SideViewWidget.h"

#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent)
    : QWidget(parent)
    , mAxisDrawer(this)
{
    initAxisDrawerParameters();
}

void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //    QBrush brush;
    //    brush.setStyle(Qt::BrushStyle::SolidPattern);
    //    brush.setColor(QColor(180, 180, 180));
    //    painter.fillRect(0, 0, width(), height(), brush);

    mAxisDrawerParameters.origin.setX(width() / 2.0);
    mAxisDrawerParameters.origin.setY(height() / 2.0);

    mAxisDrawer.draw(mAxisDrawerParameters);
}

void SideViewWidget::initAxisDrawerParameters()
{
    {
        mAxisDrawerParameters.horizontal.minorTickmarkCount = 1;
        mAxisDrawerParameters.horizontal.tickmarkValueStep = 6;

        mAxisDrawerParameters.horizontal.labelStyle.pen = QColor(0, 0, 0);
        mAxisDrawerParameters.horizontal.labelStyle.font = QFont("Arial");
        mAxisDrawerParameters.horizontal.labelStyle.font.setPixelSize(9);

        mAxisDrawerParameters.horizontal.tickmarkStyle.brush = QColor(100, 100, 100);
        mAxisDrawerParameters.horizontal.tickmarkStyle.size = QSizeF(1.5, 6);

        mAxisDrawerParameters.horizontal.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mAxisDrawerParameters.horizontal.minorTickmarkStyle.size = QSizeF(1, 4);

        mAxisDrawerParameters.horizontal.pen = QPen(QColor(0, 0, 0));
        mAxisDrawerParameters.horizontal.pen.setWidthF(1);
    }

    {
        mAxisDrawerParameters.vertical.minorTickmarkCount = 1;
        mAxisDrawerParameters.vertical.tickmarkValueStep = 6;

        mAxisDrawerParameters.vertical.labelStyle.pen = QColor(0, 0, 0);
        mAxisDrawerParameters.vertical.labelStyle.font = QFont("Arial");
        mAxisDrawerParameters.vertical.labelStyle.font.setPixelSize(9);

        mAxisDrawerParameters.vertical.tickmarkStyle.brush = QColor(100, 100, 100);
        mAxisDrawerParameters.vertical.tickmarkStyle.size = QSizeF(6, 1.5);

        mAxisDrawerParameters.vertical.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mAxisDrawerParameters.vertical.minorTickmarkStyle.size = QSizeF(4, 1);

        mAxisDrawerParameters.vertical.pen = QPen(QColor(0, 0, 0));
        mAxisDrawerParameters.vertical.pen.setWidthF(1);
    }

    mAxisDrawerParameters.valueToPixelRatio = 10;
    mAxisDrawerParameters.origin.setX(240);
    mAxisDrawerParameters.origin.setY(100);
}
