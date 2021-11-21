#include "TopViewWidget.h"

#include <QPainter>

TopViewWidget::TopViewWidget(QWidget *parent)
    : QWidget(parent)
{
    // General GUI
    {
        QVector<qreal> dashes;
        dashes << 2 << 2;
        mDashedPen.setDashPattern(dashes);
        mDashedPen.setStyle(Qt::DashLine);
        mSolidPen.setWidthF(1);

        mLabelFont = QFont("Arial");
        mLabelFont.setPixelSize(11);
        mLabelColor = QColor(100, 100, 100);

        mCrossedPatternBursh.setStyle(Qt::BrushStyle::CrossPattern);
        mCrossedPatternBursh.setColor(QColor(211, 211, 211));
    }

    // Target Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHandle.setPen(pen);
        mTargetHandle.setBrush(QColor(255, 0, 0));
        mTargetHandle.setHoveredBrush(QColor(255, 255, 0));
        mTargetHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetHandle.setSize(10, 10);
    }

    // Target Distance Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleTop.setPen(pen);
        mFovWidthHandleTop.setBrush(QColor(255, 128, 0));
        mFovWidthHandleTop.setHoveredBrush(QColor(255, 255, 0));
        mFovWidthHandleTop.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleTop.setSize(10, 10);
    }

    // Camera Height Handler
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleBottom.setPen(pen);
        mFovWidthHandleBottom.setBrush(QColor(255, 128, 0));
        mFovWidthHandleBottom.setHoveredBrush(QColor(255, 255, 0));
        mFovWidthHandleBottom.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleBottom.setSize(10, 10);
    }
}

void TopViewWidget::setParameters(Dori::Core::Controller::TopViewWidgetParamaters *newParameters)
{
    mParameters = newParameters;
}

void TopViewWidget::refresh()
{
    updateHandles();
    update();
}

QPointF TopViewWidget::mapFrom3d(Eigen::Vector3f vector)
{
    float x = mOrigin.x() + vector.x() * mMeterToPixelRatio;
    float y = mOrigin.y() - vector.y() * mMeterToPixelRatio;
    return QPointF(x, y);
}

Eigen::Vector3f TopViewWidget::mapFrom2d(QPointF point)
{
    Eigen::Vector3f vector;

    vector[0] = (point.x() - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = (mOrigin.y() - point.y()) / mMeterToPixelRatio;
    vector[2] = 0;
    return vector;
}

void TopViewWidget::updateHandles()
{
    mTargetHandle.setCenter(mapFrom3d(Eigen::Vector3f(mParameters->targetDistance, 0, 0)));
    mFovWidthHandleTop.setCenter(mParameters->target[0]);
    mFovWidthHandleBottom.setCenter(mParameters->target[3]);
}

void TopViewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.fillRect(0, 0, width(), height(), mCrossedPatternBursh);

    mTargetHandle.draw(this);
    mFovWidthHandleTop.draw(this);
    mFovWidthHandleBottom.draw(this);
}

void TopViewWidget::mousePressEvent(QMouseEvent *) {}

void TopViewWidget::mouseMoveEvent(QMouseEvent *) {}

void TopViewWidget::mouseReleaseEvent(QMouseEvent *) {}

void TopViewWidget::wheelEvent(QWheelEvent *) {}

void TopViewWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;
}

void TopViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}
