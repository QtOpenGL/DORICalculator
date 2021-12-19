#include "TargetWidget.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

TargetWidget::TargetWidget(QWidget *parent)
    : QGroupBox(parent)
{
    QGridLayout *layout = new QGridLayout;

    mLineEditTargetHeight = new QLineEdit;
    mLineEditTargetHeight->setValidator(new QDoubleValidator(0.1, 10000, 1));
    layout->addWidget(new QLabel("Height (m):"), 1, 0);
    layout->addWidget(mLineEditTargetHeight, 1, 1);
    connect(mLineEditTargetHeight, &QLineEdit::editingFinished, this, [this]() {
        mParameters->target.height = mLineEditTargetHeight->text().toFloat();
        emit dirty();
    });

    mLineEditTargetDistance = new QLineEdit;
    mLineEditTargetDistance->setValidator(new QDoubleValidator(0.1, 10000, 1));
    layout->addWidget(new QLabel("Distance (m):"), 2, 0);
    layout->addWidget(mLineEditTargetDistance, 2, 1);
    connect(mLineEditTargetDistance, &QLineEdit::editingFinished, this, [this]() {
        mParameters->target.distance = mLineEditTargetDistance->text().toFloat();
        emit dirty();
    });

    mLineEditFovWidth = new QLineEdit;
    mLineEditFovWidth->setValidator(new QDoubleValidator(0.1, 10000, 1));
    layout->addWidget(new QLabel("Fov Width (m):"), 3, 0);
    layout->addWidget(mLineEditFovWidth, 3, 1);
    connect(mLineEditFovWidth, &QLineEdit::editingFinished, this, [this]() {
        mParameters->target.fovWidth = mLineEditFovWidth->text().toFloat();
        emit dirty();
    });

    setTitle("Target");
    setLayout(layout);
}

void TargetWidget::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}

void TargetWidget::refresh()
{
    mLineEditTargetHeight->setText(QString("%1").arg(mParameters->target.height, 0, 'f', 2));
    mLineEditTargetDistance->setText(QString("%1").arg(mParameters->target.distance, 0, 'f', 2));
    mLineEditFovWidth->setText(QString("%1").arg(mParameters->target.fovWidth, 0, 'f', 2));
}
