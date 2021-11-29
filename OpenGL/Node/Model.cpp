#include "Model.h"

Model::Model(QString name)
    : mName(name)
{}

const QString &Model::name() const
{
    return mName;
}
