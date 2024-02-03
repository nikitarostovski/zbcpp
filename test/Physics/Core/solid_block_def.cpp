#include "solid_block_def.hpp"

using namespace PolygonUtils;

SolidBlockDef::SolidBlockDef(Polygon& polygon, bool canBeSplit)
    : polygon(polygon)
{
    if (canBeSplit) {
        partsDefs = makeParts();
    }
}

std::vector<SolidBlockDef> SolidBlockDef::makeParts()
{
    std::vector<SolidBlockDef> result;
    for (Polygon chunk : polygon.split(20.0f)) {
        
        SolidBlockDef newBlock{chunk, false};
        result.push_back(newBlock);
    }
    return result;
}
