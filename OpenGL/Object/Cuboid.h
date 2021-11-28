#ifndef CUBOID_H
#define CUBOID_H

#include "Object.h"
#include <QMatrix4x4>

class Cuboid : public Object
{
public:
    Cuboid(float x = 0, float y = 0, float z = 0);
};

#endif // CUBOID_H
