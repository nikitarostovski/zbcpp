#pragma once
#include "Box2D.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include "material.hpp"

namespace PolygonUtils {

struct Polygon {
private:
    void calculateCenter();
public:
    std::vector<b2Vec2> points;
    b2Vec2 center;
    b2AABB aabb;
    Material material;
    bool isDynamic;
    
    Polygon(std::vector<b2Vec2> points, MaterialType materialType, bool isDynamic);
    
    std::vector<Polygon> triangulate();
    std::vector<Polygon> splitIntoSubpolygons(); // splits concave polygon into convex
    std::vector<Polygon> split(float distance);
    std::vector<Polygon> intersection(std::vector<Polygon> source);
    
    static std::vector<Polygon> combine(std::vector<Polygon> source);
    static Polygon makeCircle(b2Vec2 center, float radius, int pointCount, MaterialType materialType, bool isDynamic);
};

}
