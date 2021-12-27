#ifndef REGIONINFOWIDGET_H
#define REGIONINFOWIDGET_H

#include "Rectangle.h"

#include <QGroupBox>
#include <QLabel>
#include <QWidget>

#include <Core/Constants.h>
#include <Core/Logic.h>

class RegionInfoWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit RegionInfoWidget(QWidget *parent = nullptr);

    void refresh();
    void setParameters(Logic::Parameters *newParameters);

private:
    QVector<Rectangle *> mRectangles;
    QVector<QLabel *> mLimitLabels;

    Logic::Parameters *mParameters;
};

#endif // REGIONINFOWIDGET_H
