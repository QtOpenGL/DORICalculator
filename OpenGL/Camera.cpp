#include "Camera.h"

Camera::Camera()
    : mPosition(0, 0, 0)
    , mDirty(false)
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
            mPosition += mRotation.rotatedVector(QVector3D(0, 0, -amount));
            break;
        case BACKWARD:
            mPosition += mRotation.rotatedVector(QVector3D(0, 0, +amount));
            break;
        case LEFT:
            mPosition += mRotation.rotatedVector(QVector3D(-amount, 0, 0));
            break;
        case RIGHT:
            mPosition += mRotation.rotatedVector(QVector3D(amount, 0, 0));
            break;
        case UP:
            mPosition += mRotation.rotatedVector(QVector3D(0, amount, 0));
            break;
        case DOWN:
            mPosition += mRotation.rotatedVector(QVector3D(0, -amount, 0));
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
        mViewMatrix.translate(-mPosition);
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
    mPosition = position;
    mDirty = true;
}

void Camera::setPosition(float x, float y, float z)
{
    mPosition = QVector3D(x, y, z);
    mDirty = true;
}

QVector3D Camera::position() const
{
    return mPosition;
}

float Camera::movementSpeed() const
{
    return mMovementSpeed;
}

void Camera::setMovementSpeed(float newMovementSpeed)
{
    mMovementSpeed = newMovementSpeed;
}
