#include "Camera.h"

Camera::Camera()
    : mDirty(false)
    , mMovementSpeed(1.0f)
{
    mProjectionMatrix.setToIdentity();
    mViewMatrix.setToIdentity();
}

void Camera::move(KeyboardControl key, float amount)
{
    mKeyboardControls.insert(key, amount);

    if (!qFuzzyCompare(amount, 0.0f))
        mDirty = true;
}

void Camera::rotate(MouseControl axis, float amount)
{
    mMouseControls.insert(axis, amount);

    if (!qFuzzyCompare(amount, 0.0f))
        mDirty = true;
}

void Camera::update()
{
    QList<Camera::MouseControl> controls = mMouseControls.keys();

    for (Camera::MouseControl control : controls) {
        float amount = mMouseControls.value(control);
        mMouseControls.insert(control, 0);

        if (qFuzzyCompare(amount, 0.0f))
            continue;

        switch (control) {
        case Camera::TILT: {
            mRotation = mRotation * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), amount);
            break;
        }
        case Camera::PAN: {
            mRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -amount) * mRotation;
            break;
        }
        }
    }

    QList<Camera::KeyboardControl> keys = mKeyboardControls.keys();

    for (Camera::KeyboardControl key : keys) {
        float amount = mMovementSpeed * mKeyboardControls.value(key);

        if (qFuzzyCompare(amount, 0.0f))
            continue;

        switch (key) {
        case FORWARD:
            mTranslation += mRotation.rotatedVector(QVector3D(0, 0, -amount));
            break;
        case BACKWARD:
            mTranslation += mRotation.rotatedVector(QVector3D(0, 0, +amount));
            break;
        case LEFT:
            mTranslation += mRotation.rotatedVector(QVector3D(-amount, 0, 0));
            break;
        case RIGHT:
            mTranslation += mRotation.rotatedVector(QVector3D(amount, 0, 0));
            break;
        case UP:
            mTranslation += mRotation.rotatedVector(QVector3D(0, amount, 0));
            break;
        case DOWN:
            mTranslation += mRotation.rotatedVector(QVector3D(0, -amount, 0));
            break;
        }
    }
}

const QMatrix4x4 &Camera::projectionMatrix() const
{
    return mProjectionMatrix;
}

void Camera::setProjectionMatrix(const QMatrix4x4 &newProjectionMatrix)
{
    mProjectionMatrix = newProjectionMatrix;
}

const QMatrix4x4 &Camera::viewMatrix() const
{
    if (mDirty) {
        mViewMatrix.setToIdentity();
        mViewMatrix.rotate(mRotation.conjugated());
        mViewMatrix.translate(-mTranslation);
        return mViewMatrix;
    }

    return mViewMatrix;
}

void Camera::setViewMatrix(const QMatrix4x4 &newViewMatrix)
{
    mViewMatrix = newViewMatrix;
}

void Camera::setPosition(const QVector3D &position)
{
    mTranslation = -position;
    mDirty = true;
}

void Camera::setPosition(float x, float y, float z)
{
    mTranslation = -QVector3D(x, y, z);
    mDirty = true;
}

float Camera::movementSpeed() const
{
    return mMovementSpeed;
}

void Camera::setMovementSpeed(float newMovementSpeed)
{
    mMovementSpeed = newMovementSpeed;
}
