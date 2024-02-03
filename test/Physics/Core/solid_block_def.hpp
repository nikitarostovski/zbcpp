#pragma once
#include "SFML/Graphics.hpp"
#include "polygon.hpp"
#include "Box2D.h"

using namespace PolygonUtils;

struct SolidBlockDef
{
private:
    std::vector<SolidBlockDef> makeParts();
    
public:
    Polygon polygon;
    
    std::vector<SolidBlockDef> partsDefs;
    
    SolidBlockDef(Polygon& polygon, bool canBeSplit);
};
