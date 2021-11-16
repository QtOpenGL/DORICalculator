#include "SideViewWidget.h"

#include <QPainter>

SideViewWidget::SideViewWidget(QWidget *parent)
    : ViewWidget(parent)
{
    initAxisDrawerParameters();
    width();
    mCameraHeight = 400;
    mTargetDistance = 300;
    mTargetHeight = 200;
}

void SideViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //    QBrush brush;
    //    brush.setStyle(Qt::BrushStyle::SolidPattern);
    //    brush.setColor(QColor(180, 180, 180));
    //    painter.fillRect(0, 0, width(), height(), brush);

    mOrigin.setX(width() / 2.0);
    mOrigin.setY(height() / 2.0);

    mAxisDrawer.draw();
}

void SideViewWidget::initAxisDrawerParameters()
{
    // Horizontal axis parameters
    {
        mHorizontalAxisParameters.minorTickmarkCount = 1;
        mHorizontalAxisParameters.tickmarkValueStep = 6;

        mHorizontalAxisParameters.labelStyle.pen = QColor(0, 0, 0);
        mHorizontalAxisParameters.labelStyle.font = QFont("Arial");
        mHorizontalAxisParameters.labelStyle.font.setPixelSize(9);

        mHorizontalAxisParameters.tickmarkStyle.brush = QColor(100, 100, 100);
        mHorizontalAxisParameters.tickmarkStyle.size = QSizeF(1.5, 6);

        mHorizontalAxisParameters.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mHorizontalAxisParameters.minorTickmarkStyle.size = QSizeF(1, 4);

        mHorizontalAxisParameters.pen = QPen(QColor(0, 0, 0));
        mHorizontalAxisParameters.pen.setWidthF(1);
    }

    // Vertical axis parameters
    {
        mVerticalAxisParameters.minorTickmarkCount = 1;
        mVerticalAxisParameters.tickmarkValueStep = 6;
        mVerticalAxisParameters.labelStyle.pen = QColor(0, 0, 0);
        mVerticalAxisParameters.labelStyle.font = QFont("Arial");
        mVerticalAxisParameters.labelStyle.font.setPixelSize(9);
        mVerticalAxisParameters.tickmarkStyle.brush = QColor(100, 100, 100);
        mVerticalAxisParameters.tickmarkStyle.size = QSizeF(6, 1.5);
        mVerticalAxisParameters.minorTickmarkStyle.brush = QColor(0, 0, 0);
        mVerticalAxisParameters.minorTickmarkStyle.size = QSizeF(4, 1);
        mVerticalAxisParameters.pen = QPen(QColor(0, 0, 0));
        mVerticalAxisParameters.pen.setWidthF(1);
    }

    mValueToPixelRatio = 10;
}
