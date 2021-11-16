#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include <GUI/Base/AxisDrawer.h>
#include <QWidget>

class SideViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideViewWidget(QWidget *parent = nullptr);

private:
    void initAxisDrawerParameters();

    void paintEvent(QPaintEvent *event) override;

    AxisDrawer::Parameters mAxisDrawerParameters;
    AxisDrawer mAxisDrawer;
};

#endif // SIDEVIEWWIDGET_H
