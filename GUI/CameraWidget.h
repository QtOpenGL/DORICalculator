#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QGroupBox>
#include <QLineEdit>

#include <Core/Logic.h>

class CameraWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);

    void setParameters(Logic::Parameters *newParameters);

public slots:
    void refresh();

signals:
    void dirty();

private:
    QLineEdit *mLineEditCameraHeight;
    QLineEdit *mLineEditTiltAngle;
    QLineEdit *mLineEditSensorWidth;
    QLineEdit *mLineEditSensorHeight;
    QLineEdit *mLineEditAspectRatio;
    QLineEdit *mLineEditHorizontalFov;
    QLineEdit *mLineEditVerticalFov;
    QLineEdit *mLineEditZNear;
    QLineEdit *mLineEditZFar;

    Logic::Parameters *mParameters;
};

#endif // CAMERAWIDGET_H
