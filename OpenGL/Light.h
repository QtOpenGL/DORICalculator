#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
public:
    Light();

    const QVector3D &position() const;
    void setPosition(float x, float y, float z);

    const QVector3D &color() const;
    void setColor(float r, float g, float b);

    float ambientStrength() const;
    void setAmbientStrength(float newAmbientStrength);

    float lightPower() const;
    void setLightPower(float newLightPower);

private:
    QVector3D mPosition;
    QVector3D mColor;
    float mAmbientStrength;
    float mLightPower;
};

#endif // LIGHT_H
