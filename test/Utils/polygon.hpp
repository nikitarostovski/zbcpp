#pragma once
#include "Box2D.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include "material.hpp"

namespace PolygonUtils {

struct Polygon {
private:
    void calculateCenter();
    void calculateAABB();
    void calculateSubPolygons();
public:
    std::vector<Polygon> subPolygons;
    std::vector<b2Vec2> points;
    b2Vec2 center;
    b2AABB aabb;
    Material material;
    bool isDynamic;
    
    Polygon(std::vector<b2Vec2> points, MaterialType materialType, bool isDynamic, bool isSplitable);
    
    std::vector<Polygon> intersection(std::vector<Polygon> source);
    std::vector<Polygon> substraction(std::vector<Polygon> source);
    
    static std::vector<Polygon> combine(std::vector<Polygon> source);
    static Polygon makeCircle(b2Vec2 center, float radius, int pointCount, MaterialType materialType, bool isDynamic, bool canBeSplit);
    
    static std::vector<Polygon> substraction_mult(std::vector<Polygon> source, std::vector<Polygon> clip);
};

}
