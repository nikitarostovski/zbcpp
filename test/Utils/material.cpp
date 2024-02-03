#include "material.hpp"

#define ALPHA 255 // 64

using namespace PolygonUtils;

sf::Color getMaterialColor(MaterialType type);

Material::Material(MaterialType type)
    : type(type)
{
    this->color = getMaterialColor(type);
}

sf::Color getMaterialColor(MaterialType type)
{
    switch (type) {
        case MaterialType::red:
            return {255, 0, 0, ALPHA};
        case MaterialType::green:
            return {0, 255, 0, ALPHA};
        case MaterialType::yellow:
            return {255, 255, 0, ALPHA};
        case MaterialType::blue:
            return {0, 0, 255, ALPHA};
    }
}
