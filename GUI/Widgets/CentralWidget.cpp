#include "CentralWidget.h"
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    mSideViewTitle = new QLabel("SideViewWidget");
    mSideViewWidget = new SideViewWidget;
    mSideViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mSideViewTitle->setAlignment(Qt::AlignLeft);

    mTopViewTitle = new QLabel("TopViewWidget");
    mTopViewTitle->setAlignment(Qt::AlignLeft);
    mTopViewWidget = new TopViewWidget;
    mTopViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mGroupBox = new QGroupBox;
    mGroupBox->setTitle("Controls");
    mGroupBox->setMinimumWidth(150);

    mBottomBarWidget = new QGroupBox;
    mBottomBarWidget->setTitle("BottomBar");
    mBottomBarWidget->setMinimumHeight(60);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;

    // Left Group Box
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(mGroupBox);
        topLayout->addLayout(layout);
    }

    // Views
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(mSideViewTitle);
        layout->addWidget(mSideViewWidget);

        layout->addWidget(mTopViewTitle);
        layout->addWidget(mTopViewWidget);
        topLayout->addLayout(layout);
    }

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(mBottomBarWidget);

    setLayout(mainLayout);
}
