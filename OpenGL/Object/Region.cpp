#include "Region.h"

Region::Region() {}

RegionData *Region::data() const { return mData; }

void Region::setData(RegionData *newData) { mData = newData; }
