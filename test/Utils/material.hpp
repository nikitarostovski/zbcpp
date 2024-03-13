#pragma once
#include "SFML/Graphics.hpp"

namespace PolygonUtils {

class Polygon;

enum MaterialType {
    // Fillers
    MaterialTypeRock,
    MaterialTypeDirt,
    
    // Valuables
    MaterialTypeCrystalRed,
    MaterialTypeCrystalGreen,
    
    // Unbreakables
    MaterialTypeCore,
    
    // Destructives
    MaterialTypeExplosive,
    MaterialTypeLava
};

static MaterialType allMaterialTypes[] = {
    MaterialTypeRock,
    MaterialTypeDirt,
    MaterialTypeCrystalRed,
    MaterialTypeCrystalGreen,
    MaterialTypeCore,
    MaterialTypeExplosive,
    MaterialTypeLava
};

struct Material {
private:
    std::vector<Polygon> splitIntoSquares(Polygon polygon, float cellSize);
    std::vector<Polygon> splitIntoTriangles(Polygon polygon, float cellSize);
    std::vector<Polygon> splitIntoHexagons(Polygon polygon, float cellSize);
public:
    MaterialType type;
    
    Material(MaterialType type);
    
    std::vector<Polygon> splitPolygon(Polygon polygon);
};

}
