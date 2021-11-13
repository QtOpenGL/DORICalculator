#ifndef SIDEVIEWWIDGET_H
#define SIDEVIEWWIDGET_H

#include <QWidget>

class SideViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideViewWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // SIDEVIEWWIDGET_H
