#include "Light.h"

Light::Light()
    : mAmbientStrength(0.1)
    , mColor(QVector3D(1, 1, 1))
    , mPosition(QVector3D(0, 0, 1))
{}

float Light::ambientStrength() const
{
    return mAmbientStrength;
}

void Light::setAmbientStrength(float newAmbientStrength)
{
    mAmbientStrength = newAmbientStrength;
}

const QVector3D &Light::color() const
{
    return mColor;
}

void Light::setColor(const QVector3D &newColor)
{
    mColor = newColor;
}

const QVector3D &Light::position() const
{
    return mPosition;
}

void Light::setPosition(const QVector3D &newPosition)
{
    mPosition = newPosition;
}

void Light::setPosition(float x, float y, float z)
{
    mPosition = QVector3D(x, y, z);
}
