#include "CentralWidget.h"
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{}

void CentralWidget::init()
{
    // SideViewWidget
    {
        mSideViewTitle = new QLabel("SideViewWidget");
        mSideViewWidget = new SideViewWidget;
        mSideViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mSideViewTitle->setAlignment(Qt::AlignLeft);
    }

    // TopViewWidget
    {
        mTopViewTitle = new QLabel("TopViewWidget");
        mTopViewTitle->setAlignment(Qt::AlignLeft);
        mTopViewWidget = new TopViewWidget;
        mTopViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    // Layouts
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(mSideViewTitle);
        layout->addWidget(mSideViewWidget);

        layout->addWidget(mTopViewTitle);
        layout->addWidget(mTopViewWidget);

        setLayout(layout);
    }
}

SideViewWidget *CentralWidget::sideViewWidget() const
{
    return mSideViewWidget;
}

TopViewWidget *CentralWidget::topViewWidget() const
{
    return mTopViewWidget;
}
