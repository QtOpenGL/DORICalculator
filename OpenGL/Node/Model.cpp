#include "Model.h"

Model::Model(QString name)
    : mName(name)
    , mColored(false)
{}

const QString &Model::name() const
{
    return mName;
}

bool Model::colored() const
{
    return mColored;
}

void Model::setColored(bool newColored)
{
    mColored = newColored;
}

const QVector3D &Model::color() const
{
    return mColor;
}

void Model::setColor(float r, float g, float b)
{
    mColor = QVector3D(r, g, b);
}
