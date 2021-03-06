#include "Light.h"

Light::Light()
    : mPosition(0, 0, 0)
    , mColor(1, 1, 1)
    , mAmbientStrength(0.1f)
    , mLightPower(1.0f)

{}

const QVector3D &Light::position() const
{
    return mPosition;
}

void Light::setPosition(float x, float y, float z)
{
    mPosition = QVector3D(x, y, z);
}

const QVector3D &Light::color() const
{
    return mColor;
}

void Light::setColor(float r, float g, float b)
{
    mColor = QVector3D(r, g, b);
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
