#include "RegionInfoWidget.h"

#include <QGridLayout>

RegionInfoWidget::RegionInfoWidget(QWidget *parent)
    : QGroupBox(parent)
{
    QGridLayout *layout = new QGridLayout;

    QStringList names;
    names << "Strong Identification"
          << "Identification"
          << "Recognition"
          << "Observation"
          << "Detection"
          << "Monitoring"
          << "Dead Zone";

    for (int i = 0; i < names.length(); ++i) {
        mRectangles << new Rectangle;
        mRectangles[i]->setColor(REGION_COLORS[i]);
        mRectangles[i]->setFixedSize(20, 20);

        mLimitLabels << new QLabel;
        mLimitLabels[i]->setAlignment(Qt::AlignRight);

        QLabel *label = new QLabel(QString("%1 ppm").arg(REGION_PPMS[i]));
        label->setAlignment(Qt::AlignRight);

        layout->addWidget(mRectangles[i], i, 0);
        layout->addWidget(new QLabel(names[i]), i, 1);
        layout->addWidget(label, i, 2);
        layout->addWidget(mLimitLabels[i], i, 3);
    }

    setLayout(layout);
    setTitle("Regions");
}

void RegionInfoWidget::refresh()
{
    for (int i = 0; i < mLimitLabels.size(); ++i) {
        mLimitLabels[i]->setText(QString("%1 m").arg(mParameters->regions[i].limit, -1, 'f', 2));
    }
}

void RegionInfoWidget::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}
