#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
public:
    Light();

    float ambientStrength() const;
    void setAmbientStrength(float newAmbientStrength);

    const QVector3D &color() const;
    void setColor(const QVector3D &newColor);

    const QVector3D &position() const;
    void setPosition(const QVector3D &newPosition);
    void setPosition(float x, float y, float z);

private:
    float mAmbientStrength;
    QVector3D mColor;
    QVector3D mPosition;
};

#endif // LIGHT_H
