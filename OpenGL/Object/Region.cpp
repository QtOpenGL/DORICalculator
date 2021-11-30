#include "Region.h"

Region::Region()
    : mVisible(false)
{}

RegionData *Region::data() const { return mData; }

void Region::setData(RegionData *newData) { mData = newData; }

bool Region::visible() const { return mVisible; }

void Region::setVisible(bool newVisible) { mVisible = newVisible; }
