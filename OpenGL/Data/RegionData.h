#ifndef REGIONDATA_H
#define REGIONDATA_H

#include "ObjectData.h"

#include <QOpenGLFunctions>

class RegionData : public ObjectData, protected QOpenGLFunctions
{
public:
    RegionData();
    ~RegionData();

    bool create() override;
    void update() override;
};

#endif // REGIONDATA_H
