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
#include <OpenGL/OpenGLWidget.h>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

    void init();

    void setSideViewWidget(SideViewWidget *newSideViewWidget);
    void setTopViewWidget(TopViewWidget *newTopViewWidget);
    void setAxisWidget(AxisWidget *newAxisWidget);
    void setLeftWidget(LeftWidget *newLeftWidget);
    void setOpenGLWidget(OpenGLWidget *newOpenGLWidget);

private:
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    AxisWidget *mAxisWidget;
    LeftWidget *mLeftWidget;
    OpenGLWidget *mOpenGLWidget;
    QGroupBox *mOpenGLWidgetContainer;

    QLabel *mSideViewTitle;
    QLabel *mTopViewTitle;
};

#endif // CENTRALWIDGET_H
