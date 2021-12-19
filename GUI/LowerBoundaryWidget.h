#ifndef LOWERBOUNDARYWIDGET_H
#define LOWERBOUNDARYWIDGET_H

#include <QGroupBox>
#include <QLineEdit>
#include <QWidget>

#include <Core/Logic.h>

class LowerBoundaryWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit LowerBoundaryWidget(QWidget *parent = nullptr);

    void setParameters(Logic::Parameters *newParameters);

public slots:
    void refresh();

signals:
    void dirty();

private:
    QLineEdit *mLineEditLowerBoundaryHeight;
    QLineEdit *mLineEditLowerBoundaryDistance;

    Logic::Parameters *mParameters;
};

#endif // LOWERBOUNDARYWIDGET_H
