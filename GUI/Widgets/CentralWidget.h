#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "SideViewWidget.h"
#include "TopViewWidget.h"

#include <QGroupBox>
#include <QLabel>
#include <QStatusBar>
#include <QWidget>

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
};

#endif // CENTRALWIDGET_H
