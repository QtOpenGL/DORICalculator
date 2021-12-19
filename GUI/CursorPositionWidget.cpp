#include "CursorPositionWidget.h"

#include <QGridLayout>
#include <QLabel>

CursorPositionWidget::CursorPositionWidget(QWidget *parent)
    : QGroupBox(parent)
{
    QGridLayout *layout = new QGridLayout;

    mLabelCursorX = new QLabel;
    layout->addWidget(new QLabel("X:"), 1, 0);
    layout->addWidget(mLabelCursorX, 1, 1);

    mLabelCursorY = new QLabel;
    layout->addWidget(new QLabel("Y:"), 2, 0);
    layout->addWidget(mLabelCursorY, 2, 1);

    setTitle("Cursor Position");
    setLayout(layout);
}

void CursorPositionWidget::onCursorPositionChanged(QPointF position)
{
    mLabelCursorX->setText(QString("%1 m").arg(position.x(), 0, 'f', 3));
    mLabelCursorY->setText(QString("%1 m").arg(position.y(), 0, 'f', 3));
}
