#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

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
    void init();

    SideViewWidget *sideViewWidget() const;
    TopViewWidget *topViewWidget() const;

private:
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;

    QLabel *mSideViewTitle;
    QLabel *mTopViewTitle;
};

#endif // CENTRALWIDGET_H
