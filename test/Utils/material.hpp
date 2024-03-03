#pragma once
#include "SFML/Graphics.hpp"

namespace PolygonUtils {

class Polygon;

enum MaterialType {
    red,
    green,
    yellow,
    blue
};

struct Material {
private:
    std::vector<Polygon> splitIntoSquares(Polygon polygon);
    std::vector<Polygon> splitIntoTriangles(Polygon polygon);
public:
    MaterialType type;
    sf::Color color;
    
    Material(MaterialType type);
    
    std::vector<Polygon> splitPolygon(Polygon polygon);
};

}
