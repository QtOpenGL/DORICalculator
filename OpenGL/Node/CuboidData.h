#ifndef CUBOIDDATA_H
#define CUBOIDDATA_H

#include "BasicObjectData.h"

class CuboidData : public BasicObjectData
{
public:
    CuboidData();

    void createVertexData() override;
};

#endif // CUBOIDDATA_H
