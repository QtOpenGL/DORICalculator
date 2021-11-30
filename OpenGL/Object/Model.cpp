#include "Model.h"

Model::Model(Type type)
    : mType(type)
{}

Model::Type Model::type() const { return mType; }
