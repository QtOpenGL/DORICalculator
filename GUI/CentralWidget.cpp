#include "CentralWidget.h"
#include <QStackedLayout>
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{}

void CentralWidget::init()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *viewLayout = new QVBoxLayout;
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // SideViewWidget and AxisWidget
    {
        mSideViewTitle = new QLabel("SideViewWidget");
        mSideViewTitle->setAlignment(Qt::AlignLeft);
        viewLayout->addWidget(mSideViewTitle);

        QGridLayout *layout = new QGridLayout;
        mAxisWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mAxisWidget, 0, 0);

        mSideViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mSideViewWidget, 0, 0);

        viewLayout->addLayout(layout);
    }

    // TopViewWidget
    {
        mTopViewTitle = new QLabel("TopViewWidget");
        mTopViewTitle->setAlignment(Qt::AlignLeft);
        viewLayout->addWidget(mTopViewTitle);

        mTopViewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        viewLayout->addWidget(mTopViewWidget);
    }

    // LeftWidget
    {
        leftLayout->addWidget(mLeftWidget);
        mLeftWidget->setFixedWidth(300);
    }

    // OpenGLWidget

    {
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(mOpenGLWidget);

        mOpenGLWidgetContainer = new QGroupBox;
        mOpenGLWidgetContainer->setTitle("3D View");
        mOpenGLWidgetContainer->setLayout(layout);

        leftLayout->addWidget(mOpenGLWidgetContainer);
        mOpenGLWidgetContainer->setFixedWidth(300);
        mOpenGLWidgetContainer->setMinimumHeight(200);
    }

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(viewLayout);
    setLayout(mainLayout);
}

void CentralWidget::setSideViewWidget(SideViewWidget *newSideViewWidget) { mSideViewWidget = newSideViewWidget; }

void CentralWidget::setTopViewWidget(TopViewWidget *newTopViewWidget) { mTopViewWidget = newTopViewWidget; }

void CentralWidget::setAxisWidget(AxisWidget *newAxisWidget) { mAxisWidget = newAxisWidget; }

void CentralWidget::setLeftWidget(LeftWidget *newLeftWidget) { mLeftWidget = newLeftWidget; }

void CentralWidget::setOpenGLWidget(OpenGLWidget *newOpenGLWidget) { mOpenGLWidget = newOpenGLWidget; }
