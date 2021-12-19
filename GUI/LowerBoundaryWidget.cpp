#include "LowerBoundaryWidget.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>

LowerBoundaryWidget::LowerBoundaryWidget(QWidget *parent)
    : QGroupBox(parent)
{
    QGridLayout *layout = new QGridLayout;

    mLineEditLowerBoundaryHeight = new QLineEdit;
    mLineEditLowerBoundaryHeight->setValidator(new QDoubleValidator(0.01, 10000, 2));
    layout->addWidget(new QLabel("Height (m):"), 1, 0);
    layout->addWidget(mLineEditLowerBoundaryHeight, 1, 1);
    connect(mLineEditLowerBoundaryHeight, &QLineEdit::editingFinished, this, [this]() {
        mParameters->lowerBoundary.height = mLineEditLowerBoundaryHeight->text().toFloat();
        emit dirty();
    });

    mLineEditLowerBoundaryDistance = new QLineEdit;
    mLineEditLowerBoundaryDistance->setEnabled(false);
    mLineEditLowerBoundaryDistance->setValidator(new QDoubleValidator(0.01, 10000, 2));
    layout->addWidget(new QLabel("Distance (m):"), 2, 0);
    layout->addWidget(mLineEditLowerBoundaryDistance, 2, 1);

    setTitle("Lower Boundary");
    setLayout(layout);
}

void LowerBoundaryWidget::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}

void LowerBoundaryWidget::refresh()
{
    mLineEditLowerBoundaryHeight->setText(QString("%1").arg(mParameters->lowerBoundary.height, 0, 'f', 2));
    mLineEditLowerBoundaryDistance->setText(QString("%1").arg(mParameters->lowerBoundary.distance, 0, 'f', 2));
}
