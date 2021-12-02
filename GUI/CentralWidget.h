#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "LeftWidget.h"
#include "SideViewWidget.h"
#include "TopViewWidget.h"

#include <QGroupBox>
#include <QLabel>
#include <QStatusBar>
#include <QWidget>

#include <Core/Controller.h>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

    SideViewWidget *sideViewWidget() const;
    TopViewWidget *topViewWidget() const;
    AxisWidget *axisWidget() const;
    LeftWidget *leftWidget() const;

private:
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    AxisWidget *mAxisWidget;
    LeftWidget *mLeftWidget;

    QLabel *mSideViewTitle;
    QLabel *mTopViewTitle;
};

#endif // CENTRALWIDGET_H
