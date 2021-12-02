#include "CentralWidget.h"
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *viewLayout = new QVBoxLayout;
    // SideViewWidget
    {
        mSideViewTitle = new QLabel("SideViewWidget");
        mSideViewTitle->setAlignment(Qt::AlignLeft);
        viewLayout->addWidget(mSideViewTitle);

        QGridLayout *layout = new QGridLayout;
        mAxisWidget = new AxisWidget;
        mAxisWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mAxisWidget, 0, 0);

        mSideViewWidget = new SideViewWidget;
        mSideViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mSideViewWidget, 0, 0);

        viewLayout->addLayout(layout);
    }

    // TopViewWidget
    {
        mTopViewTitle = new QLabel("TopViewWidget");
        mTopViewTitle->setAlignment(Qt::AlignLeft);
        viewLayout->addWidget(mTopViewTitle);

        mTopViewWidget = new TopViewWidget;
        mTopViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        viewLayout->addWidget(mTopViewWidget);
    }

    {
        mLeftWidget = new LeftWidget;
        mainLayout->addWidget(mLeftWidget);
    }

    mainLayout->addLayout(viewLayout);
    setLayout(mainLayout);
}

SideViewWidget *CentralWidget::sideViewWidget() const { return mSideViewWidget; }

TopViewWidget *CentralWidget::topViewWidget() const { return mTopViewWidget; }

AxisWidget *CentralWidget::axisWidget() const { return mAxisWidget; }

LeftWidget *CentralWidget::leftWidget() const { return mLeftWidget; }
