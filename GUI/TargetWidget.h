#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include <QGroupBox>
#include <QLineEdit>

#include <Core/Logic.h>

class TargetWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit TargetWidget(QWidget *parent = nullptr);

    void setParameters(Logic::Parameters *newParameters);

public slots:
    void refresh();

signals:
    void dirty();

private:
    QLineEdit *mLineEditTargetHeight;
    QLineEdit *mLineEditTargetDistance;
    QLineEdit *mLineEditFovWidth;

    Logic::Parameters *mParameters;
};

#endif // TARGETWIDGET_H
