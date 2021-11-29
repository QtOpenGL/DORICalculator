#include "BasicObject.h"

BasicObject::BasicObject(Type type)
    : mType(type)
    , mColor(1, 1, 1)
{}

const QVector3D &BasicObject::color() const
{
    return mColor;
}

void BasicObject::setColor(float r, float g, float b)
{
    mColor = QVector3D(r, g, b);
}

BasicObject::Type BasicObject::type() const
{
    return mType;
}

void BasicObject::setType(BasicObject::Type newType)
{
    mType = newType;
}
