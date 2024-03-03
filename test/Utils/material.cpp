#include "material.hpp"
#include "polygon.hpp"

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

std::vector<Polygon> Material::splitPolygon(Polygon polygon)
{
    switch (type) {
        case MaterialType::yellow:
            return splitIntoSquares(polygon);
        case MaterialType::green:
            return splitIntoTriangles(polygon);
        default:
            std::vector<Polygon> result;
            result.push_back(polygon);
            return result;
    }
}

std::vector<Polygon> Material::splitIntoSquares(Polygon polygon)
{
    std::vector<Polygon> source;
    source.push_back(polygon);
    
    std::vector<Polygon> result;
    
    const float stepx = 3.0f;
    const float stepy = 3.0f;
    float xLo = polygon.aabb.lowerBound.x;
    float xHi = xLo + std::ceil((polygon.aabb.upperBound.x - polygon.aabb.lowerBound.x) / stepx) * stepx;
    float yLo = polygon.aabb.lowerBound.y;
    float yHi = yLo + std::ceil((polygon.aabb.upperBound.y - polygon.aabb.lowerBound.y) / stepy) * stepy;
    
    bool evenRow = true;
    for (float x = xLo; x <= xHi; x += stepx) {
        evenRow = !evenRow;
        for (float y = yLo - (evenRow ? stepy / 2 : 0); y <= yHi; y += stepy) {
            std::vector<b2Vec2> points;
            points.emplace_back(x, y);
            points.emplace_back(x + stepx, y);
            points.emplace_back(x + stepx, y + stepy);
            points.emplace_back(x, y + stepy);

            Polygon subPolygon = Polygon(points, type, polygon.isDynamic, false);
            
            for (auto clippedPolygon : subPolygon.intersection(source)) {
                result.push_back(clippedPolygon);
            }
        }
    }
    
    return result;
}

std::vector<Polygon> Material::splitIntoTriangles(Polygon polygon)
{
    std::vector<Polygon> source;
    source.push_back(polygon);
    
    std::vector<Polygon> result;
    
    const float stepx = 3.0f;
    const float stepy = stepx * sqrt(3) / 2;
    float xLo = polygon.aabb.lowerBound.x;
    float xHi = xLo + std::ceil((polygon.aabb.upperBound.x - polygon.aabb.lowerBound.x) / stepx) * stepx;
    float yLo = polygon.aabb.lowerBound.y;
    float yHi = yLo + std::ceil((polygon.aabb.upperBound.y - polygon.aabb.lowerBound.y) / stepy) * stepy;
    
    for (float x = xLo; x <= xHi; x += stepx) {
        for (float y = yLo; y <= yHi; y += stepy) {
            std::vector<b2Vec2> points;
            points.emplace_back(x, y);
            points.emplace_back(x + stepx, y);
            points.emplace_back(x + stepx / 2, y + stepy);
            Polygon subPolygon1 = Polygon(points, type, polygon.isDynamic, false);
            for (auto clippedPolygon : subPolygon1.intersection(source)) {
                result.push_back(clippedPolygon);
            }
            std::vector<b2Vec2> points2;
            points2.emplace_back(x + stepx, y);
            points2.emplace_back(x + stepx / 2, y + stepy);
            points2.emplace_back(x + stepx * 3 / 2, y + stepy);
            Polygon subPolygon2 = Polygon(points2, type, polygon.isDynamic, false);
            for (auto clippedPolygon : subPolygon2.intersection(source)) {
                result.push_back(clippedPolygon);
            }
        }
    }
    
    return result;
}
