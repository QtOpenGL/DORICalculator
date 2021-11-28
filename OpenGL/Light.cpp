#include "Light.h"

Light::Light()
    : mPosition(0, 0, 0)
    , mColor(1, 1, 1)
    , mAmbientStrength(0.1f)
    , mLightPower(160.0f)

{}

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

const QVector3D &Light::color() const
{
    return mColor;
}

void Light::setColor(const QVector3D &newColor)
{
    mColor = newColor;
}

float Light::ambientStrength() const
{
    return mAmbientStrength;
}

void Light::setAmbientStrength(float newAmbientStrength)
{
    mAmbientStrength = newAmbientStrength;
}

float Light::lightPower() const
{
    return mLightPower;
}

void Light::setLightPower(float newLightPower)
{
    mLightPower = newLightPower;
}
