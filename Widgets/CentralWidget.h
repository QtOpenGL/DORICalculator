#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QGroupBox>
#include <QLabel>
#include <QStatusBar>
#include <QWidget>
#include "SideViewWidget.h"
#include "TopViewWidget.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

signals:

private:
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    QLabel *mSideViewTitle, *mTopViewTitle;
    QGroupBox *mGroupBox;
    QGroupBox *mBottomBarWidget;
};

#endif // CENTRALWIDGET_H
