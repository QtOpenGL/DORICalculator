#include "CentralWidget.h"
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{}

void CentralWidget::init()
{
    QVBoxLayout *containerLayout = new QVBoxLayout;

    // SideViewWidget
    {
        mSideViewTitle = new QLabel("SideViewWidget");
        mSideViewTitle->setAlignment(Qt::AlignLeft);
        containerLayout->addWidget(mSideViewTitle);

        QGridLayout *layout = new QGridLayout;
        mAxisWidget = new AxisWidget;
        mAxisWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mAxisWidget, 0, 0);

        mSideViewWidget = new SideViewWidget;
        mSideViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mSideViewWidget, 0, 0);

        containerLayout->addLayout(layout);
    }

    // TopViewWidget
    {
        mTopViewTitle = new QLabel("TopViewWidget");
        mTopViewTitle->setAlignment(Qt::AlignLeft);
        containerLayout->addWidget(mTopViewTitle);

        mTopViewWidget = new TopViewWidget;
        mTopViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        containerLayout->addWidget(mTopViewWidget);
    }

    setLayout(containerLayout);
}

SideViewWidget *CentralWidget::sideViewWidget() const
{
    return mSideViewWidget;
}

TopViewWidget *CentralWidget::topViewWidget() const
{
    return mTopViewWidget;
}

AxisWidget *CentralWidget::axisWidget() const
{
    return mAxisWidget;
}
