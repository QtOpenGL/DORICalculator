#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    enum KeyboardControl { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };
    enum MouseControl { TILT, PAN };

    void move(KeyboardControl control, float amount);
    void rotate(MouseControl control, float amount);
    void update();

    const QMatrix4x4 &projectionMatrix() const;
    void setProjectionMatrix(const QMatrix4x4 &newProjectionMatrix);

    const QMatrix4x4 &viewMatrix() const;
    void setViewMatrix(const QMatrix4x4 &newViewMatrix);

    void setPosition(const QVector3D &position);
    void setPosition(float x, float y, float z);

    float movementSpeed() const;
    void setMovementSpeed(float newMovementSpeed);

private:
    QQuaternion mRotation;
    QVector3D mTranslation;
    QMap<KeyboardControl, float> mKeyboardControls;
    QMap<MouseControl, float> mMouseControls;
    QMatrix4x4 mProjectionMatrix;
    mutable QMatrix4x4 mViewMatrix;
    bool mDirty;
    float mMovementSpeed;
};

#endif // CAMERA_H
