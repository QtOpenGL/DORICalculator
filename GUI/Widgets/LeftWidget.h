#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <Core/Logic.h>
#include <QGroupBox>
#include <QLineEdit>
#include <QWidget>

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);
    void refresh();

    Logic::Parameters *parameters() const;
    void setParameters(Logic::Parameters *newParameters);

signals:
    void dirty();

private:
    QGroupBox *mCameraGroupBox;
    QGroupBox *mTargetGroupBox;
    QGroupBox *mLowerBoundaryGroupBox;
    QGroupBox *CursorInfoGroupBox;

    QLineEdit *mLineEditCameraHeight;
    QLineEdit *mLineEditTiltAngle;
    QLineEdit *mLineEditSensorWidth;
    QLineEdit *mLineEditSensorHeight;
    QLineEdit *mLineEditAspectRatio;
    QLineEdit *mLineEditHorizontalFov;
    QLineEdit *mLineEditVerticalFov;
    QLineEdit *mLineEditZNear;
    QLineEdit *mLineEditZFar;
    QLineEdit *mLineEditTargetHeight;
    QLineEdit *mLineEditTargetDistance;
    QLineEdit *mLineEditFovWidth;
    QLineEdit *mLineEditLowerBoundaryHeight;
    QLineEdit *mLineEditLowerBoundaryDistance;

    Logic::Parameters *mParameters;
};

#endif // LEFTWIDGET_H
