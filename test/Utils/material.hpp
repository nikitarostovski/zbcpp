#pragma once
#include "SFML/Graphics.hpp"

namespace PolygonUtils {

enum MaterialType {
    red,
    green,
    yellow,
    blue
};

struct Material {
public:
    MaterialType type;
    sf::Color color;
    
    Material(MaterialType type);
};

}
