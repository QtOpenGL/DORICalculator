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

private:
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    QLabel *mSideViewTitle, *mTopViewTitle;
    QGroupBox *mGroupBox;
    QGroupBox *mBottomBarWidget;
    Dori::Core::Controller *mController;
};

#endif // CENTRALWIDGET_H
