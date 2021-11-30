#ifndef REGION_H
#define REGION_H

#include "Object.h"

#include <OpenGL/Data/RegionData.h>

class Region : public Object
{
public:
    Region();

    RegionData *data() const;
    void setData(RegionData *newData);

private:
    RegionData *mData; // Each region has its own data
};

#endif // REGION_H
