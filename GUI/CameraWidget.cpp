#include "CameraWidget.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>

CameraWidget::CameraWidget(QWidget *parent)
    : QGroupBox(parent)
{
    QGridLayout *layout = new QGridLayout;

    mLineEditCameraHeight = new QLineEdit;
    mLineEditCameraHeight->setValidator(new QDoubleValidator(0.1, 1000, 2));
    layout->addWidget(new QLabel("Height (m):"), 0, 0);
    layout->addWidget(mLineEditCameraHeight, 0, 1);
    connect(mLineEditCameraHeight, &QLineEdit::editingFinished, this, [this]() {
        mParameters->camera.height = mLineEditCameraHeight->text().toFloat();
        emit dirty();
    });

    mLineEditTiltAngle = new QLineEdit;
    mLineEditTiltAngle->setEnabled(false);
    layout->addWidget(new QLabel("Tilt Angle (°):"), 1, 0);
    layout->addWidget(mLineEditTiltAngle, 1, 1);

    mLineEditSensorWidth = new QLineEdit;
    mLineEditSensorWidth->setValidator(new QIntValidator(1, 10000));
    layout->addWidget(new QLabel("Sensor Width (px):"), 2, 0);
    layout->addWidget(mLineEditSensorWidth, 2, 1);
    connect(mLineEditSensorWidth, &QLineEdit::editingFinished, this, [this]() {
        mParameters->camera.sensor.width = mLineEditSensorWidth->text().toInt();
        emit dirty();
    });

    mLineEditSensorHeight = new QLineEdit;
    mLineEditSensorHeight->setValidator(new QIntValidator(1, 10000));
    layout->addWidget(new QLabel("Sensor Height (px):"), 3, 0);
    layout->addWidget(mLineEditSensorHeight, 3, 1);
    connect(mLineEditSensorHeight, &QLineEdit::editingFinished, this, [this]() {
        mParameters->camera.sensor.height = mLineEditSensorHeight->text().toInt();
        emit dirty();
    });

    mLineEditAspectRatio = new QLineEdit;
    mLineEditAspectRatio->setEnabled(false);
    layout->addWidget(new QLabel("Aspect Ratio:"), 4, 0);
    layout->addWidget(mLineEditAspectRatio, 4, 1);

    mLineEditHorizontalFov = new QLineEdit;
    mLineEditHorizontalFov->setValidator(new QDoubleValidator(0.1, 179, 2));
    layout->addWidget(new QLabel("Horizontal Fov (°):"), 5, 0);
    layout->addWidget(mLineEditHorizontalFov, 5, 1);
    connect(mLineEditHorizontalFov, &QLineEdit::editingFinished, this, [this]() {
        mParameters->frustum.horizontalFov = mLineEditHorizontalFov->text().toFloat();
        emit dirty();
    });

    mLineEditVerticalFov = new QLineEdit;
    mLineEditVerticalFov->setEnabled(false);
    layout->addWidget(new QLabel("Vertical Fov (°):"), 6, 0);
    layout->addWidget(mLineEditVerticalFov, 6, 1);

    mLineEditZNear = new QLineEdit;
    mLineEditZNear->setValidator(new QDoubleValidator(0.1, 1000, 1));
    layout->addWidget(new QLabel("Z Near (m):"), 7, 0);
    layout->addWidget(mLineEditZNear, 7, 1);
    connect(mLineEditZNear, &QLineEdit::editingFinished, this, [this]() {
        mParameters->frustum.zNear = mLineEditZNear->text().toFloat();
        emit dirty();
    });

    mLineEditZFar = new QLineEdit;
    mLineEditZFar->setValidator(new QDoubleValidator(0.1, 10000, 1));
    layout->addWidget(new QLabel("Z Far (m):"), 8, 0);
    layout->addWidget(mLineEditZFar, 8, 1);
    connect(mLineEditZFar, &QLineEdit::editingFinished, this, [this]() {
        mParameters->frustum.zFar = mLineEditZFar->text().toFloat();
        emit dirty();
    });

    setTitle("Camera");
    setLayout(layout);
}

void CameraWidget::refresh()
{
    mLineEditCameraHeight->setText(QString("%1").arg(mParameters->camera.height, 0, 'f', 2));
    mLineEditTiltAngle->setText(QString("%1").arg(mParameters->camera.tiltAngle, 0, 'f', 2));
    mLineEditSensorWidth->setText(QString("%1").arg(mParameters->camera.sensor.width, 0, 'f', 0));
    mLineEditSensorHeight->setText(QString("%1").arg(mParameters->camera.sensor.height, 0, 'f', 0));
    mLineEditAspectRatio->setText(QString("%1").arg(mParameters->camera.sensor.aspectRatio, 0, 'f', 2));
    mLineEditHorizontalFov->setText(QString("%1").arg(mParameters->frustum.horizontalFov, 0, 'f', 2));
    mLineEditVerticalFov->setText(QString("%1").arg(mParameters->frustum.verticalFov, 0, 'f', 2));
    mLineEditZNear->setText(QString("%1").arg(mParameters->frustum.zNear, 0, 'f', 2));
    mLineEditZFar->setText(QString("%1").arg(mParameters->frustum.zFar, 0, 'f', 2));
}

void CameraWidget::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}
