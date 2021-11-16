#ifndef TOPVIEWWIDGET_H
#define TOPVIEWWIDGET_H

#include "ViewWidget.h"

#include <QWidget>

class TopViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopViewWidget(QWidget *parent = nullptr);

signals:

private:
    void paintEvent(QPaintEvent *event) override;
};

#endif // TOPVIEWWIDGET_H
