#define JC_VORONOI_IMPLEMENTATION
#define JC_VORONOI_CLIP_IMPLEMENTATION

#include "polygon.hpp"
#include <random>
#include <future>
#include <functional>
#include "jc_voronoi.h"
#include "jc_voronoi_clip.h"
#include "poisson_disc_distribution.hpp"
#include "clipper.h"
#include "CDT.h"
#include "constants.h"
#include "bayazit.h"

#define UPSCALE 1.0f//100.0f

using namespace PolygonUtils;

Polygon::Polygon(std::vector<b2Vec2> points, MaterialType materialType, bool isDynamic, bool isSplitable)
    : material{materialType}
{
    this->isDynamic = isDynamic;
    this->points = points;
    this->aabb = b2AABB();
    
    calculateAABB();
    calculateCenter();
    if (isSplitable)
        calculateSubPolygons();
}

void Polygon::calculateAABB()
{
    aabb = b2AABB();
    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    for (auto p : points) {
        aabb.lowerBound.x = std::min(aabb.lowerBound.x, p.x);
        aabb.lowerBound.y = std::min(aabb.lowerBound.y, p.y);
        aabb.upperBound.x = std::max(aabb.upperBound.x, p.x);
        aabb.upperBound.y = std::max(aabb.upperBound.y, p.y);
    }
}

void Polygon::calculateCenter()
{
    b2Vec2 centroid = {0, 0};
    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  // Partial signed area

    // For all vertices
    int i = 0;
    for (i = 0; i < points.size(); ++i)
    {
        x0 = points[i].x;
        y0 = points[i].y;
        x1 = points[(i+1) % points.size()].x;
        y1 = points[(i+1) % points.size()].y;
        a = x0*y1 - x1*y0;
        signedArea += a;
        centroid.x += (x0 + x1)*a;
        centroid.y += (y0 + y1)*a;
    }

    signedArea *= 0.5;
    centroid.x /= (6.0*signedArea);
    centroid.y /= (6.0*signedArea);

    center = centroid;
    
    for (int i = 0; i < points.size(); i++) {
        points[i].x -= center.x;
        points[i].y -= center.y;
    }
}

void Polygon::calculateSubPolygons()
{
    std::vector<Polygon> source;
    source.push_back(*this);
    
    std::vector<Polygon> result;
    
    // triangles
//    const float stepx = 2.0f;
//    const float stepy = stepx * sqrt(3) / 2;
//    float xLo = aabb.lowerBound.x;
//    float xHi = xLo + std::ceil((aabb.upperBound.x - aabb.lowerBound.x) / stepx) * stepx;
//    float yLo = aabb.lowerBound.y;
//    float yHi = yLo + std::ceil((aabb.upperBound.y - aabb.lowerBound.y) / stepy) * stepy;
//    
//    for (float x = xLo; x <= xHi; x += stepx) {
//        for (float y = yLo; y <= yHi; y += stepy) {
//            std::vector<b2Vec2> points;
//            points.emplace_back(x, y);
//            points.emplace_back(x + stepx, y);
//            points.emplace_back(x + stepx / 2, y + stepy);
//            Polygon subPolygon1 = Polygon(points, material.type, isDynamic, false);
//            for (auto clippedPolygon : subPolygon1.intersection(source)) {
//                result.push_back(clippedPolygon);
//            }
//            std::vector<b2Vec2> points2;
//            points2.emplace_back(x + stepx, y);
//            points2.emplace_back(x + stepx / 2, y + stepy);
//            points2.emplace_back(x + stepx * 3 / 2, y + stepy);
//            Polygon subPolygon2 = Polygon(points2, material.type, isDynamic, false);
//            for (auto clippedPolygon : subPolygon2.intersection(source)) {
//                result.push_back(clippedPolygon);
//            }
//        }
//    }
    
    // squares
    const float stepx = 1.0f;
    const float stepy = 1.0f;
    float xLo = aabb.lowerBound.x;
    float xHi = xLo + std::ceil((aabb.upperBound.x - aabb.lowerBound.x) / stepx) * stepx;
    float yLo = aabb.lowerBound.y;
    float yHi = yLo + std::ceil((aabb.upperBound.y - aabb.lowerBound.y) / stepy) * stepy;
    
    bool evenRow = true;
    for (float x = xLo; x <= xHi; x += stepx) {
        evenRow = !evenRow;
        for (float y = yLo - (evenRow ? stepy / 2 : 0); y <= yHi; y += stepy) {
            std::vector<b2Vec2> points;
            points.emplace_back(x, y);
            points.emplace_back(x + stepx, y);
            points.emplace_back(x + stepx, y + stepy);
            points.emplace_back(x, y + stepy);

            Polygon subPolygon = Polygon(points, material.type, isDynamic, false);
            
            for (auto clippedPolygon : subPolygon.intersection(source)) {
                result.push_back(clippedPolygon);
            }
        }
    }
    
    subPolygons = result;
}

std::vector<Polygon> Polygon::combine(std::vector<Polygon> source)
{
    MaterialType material = MaterialType::red;
    bool isDynamic = false;
    
    Clipper2Lib::PathsD subject, clip, solution;
    for (auto p : source) {
        isDynamic = p.isDynamic;
        material = p.material.type;
        Clipper2Lib::PathD s;
        for (auto pt : p.points)
            s.emplace_back((p.center.x + pt.x) * UPSCALE, (p.center.y + pt.y) * UPSCALE);
        
        subject.push_back(s);
    }
    solution = Clipper2Lib::Union(subject, clip, Clipper2Lib::FillRule::EvenOdd);
    std::vector<Polygon> result;
    for (auto s : solution) {
        std::vector<b2Vec2> r;
        for (auto pt : s)
            r.emplace_back(pt.x / UPSCALE, pt.y / UPSCALE);
        
        Polygon p{r, material, isDynamic, false};
        result.push_back(p);
    }
    return result;
}

std::vector<Polygon> Polygon::intersection(std::vector<Polygon> source)
{
    Clipper2Lib::PathsD subject, clip, solution;
    
    Clipper2Lib::PathD s;
    for (auto pt : points)
        s.emplace_back((center.x + pt.x) * UPSCALE, (center.y + pt.y) * UPSCALE);
    subject.push_back(s);
    
    for (auto p : source) {
        Clipper2Lib::PathD s;
        for (auto pt : p.points)
            s.emplace_back((p.center.x + pt.x) * UPSCALE, (p.center.y + pt.y) * UPSCALE);
        
        clip.push_back(s);
    }
    solution = Clipper2Lib::Intersect(subject, clip, Clipper2Lib::FillRule::NonZero);
    std::vector<Polygon> result;
    for (auto s : solution) {
        std::vector<b2Vec2> r;
        for (auto pt : s)
            r.emplace_back(pt.x / UPSCALE, pt.y / UPSCALE);
        
        Polygon p{r, material.type, isDynamic, false};
        result.push_back(p);
    }
    return result;
}

std::vector<Polygon> Polygon::substraction(std::vector<Polygon> source)
{
    Clipper2Lib::PathsD subject, clip, solution;
    
    Clipper2Lib::PathD s;
    for (auto pt : points)
        s.emplace_back((center.x + pt.x) * UPSCALE, (center.y + pt.y) * UPSCALE);
    subject.push_back(s);
    
    for (auto p : source) {
        Clipper2Lib::PathD s;
        for (auto pt : p.points)
            s.emplace_back((p.center.x + pt.x) * UPSCALE, (p.center.y + pt.y) * UPSCALE);
        
        clip.push_back(s);
    }
    solution = Clipper2Lib::Difference(subject, clip, Clipper2Lib::FillRule::NonZero);
    std::vector<Polygon> result;
    for (auto s : solution) {
        std::vector<b2Vec2> r;
        for (auto pt : s)
            r.emplace_back(pt.x / UPSCALE, pt.y / UPSCALE);
        
        Polygon p{r, material.type, isDynamic, false};
        result.push_back(p);
    }
    return result;
}

std::vector<Polygon> Polygon::substraction_mult(std::vector<Polygon> source, std::vector<Polygon> clip)
{
    Clipper2Lib::PathsD sourcePath, clipPath, solutionPath;
    
    for (auto p : clip) {
        Clipper2Lib::PathD s;
        for (auto pt : p.points)
            s.emplace_back((p.center.x + pt.x) * UPSCALE, (p.center.y + pt.y) * UPSCALE);
        
        clipPath.push_back(s);
    }
    
    for (auto p : source) {
        Clipper2Lib::PathD s;
        for (auto pt : p.points)
            s.emplace_back((p.center.x + pt.x) * UPSCALE, (p.center.y + pt.y) * UPSCALE);
        
        sourcePath.push_back(s);
    }
    solutionPath = Clipper2Lib::Difference(sourcePath, clipPath, Clipper2Lib::FillRule::NonZero);
    std::vector<Polygon> result;
    for (auto s : solutionPath) {
        std::vector<b2Vec2> r;
        for (auto pt : s)
            r.emplace_back(pt.x / UPSCALE, pt.y / UPSCALE);
        
        Polygon p{r, MaterialType::red, true, true};
        result.push_back(p);
    }
    return result;
}

Polygon Polygon::makeCircle(b2Vec2 center, float radius, int pointCount, MaterialType materialType, bool isDynamic, bool canBeSplit)
{
    std::vector<b2Vec2> points;
    for (int i = 0; i < pointCount; i++) {
        float angle = M_PI_4 + (2 * M_PI / pointCount) * i;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        points.push_back(b2Vec2(x + center.x, y + center.y));
    }
    
    return Polygon{points, materialType, isDynamic, canBeSplit};
}
