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

#define UPSCALE 100.0f

using namespace PolygonUtils;

Polygon::Polygon(std::vector<b2Vec2> points, MaterialType materialType, bool isDynamic)
    : material{materialType}
{
    this->isDynamic = isDynamic;
    this->points = points;
    this->aabb = b2AABB();
    
    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    for (auto p : points) {
        aabb.lowerBound.x = std::min(aabb.lowerBound.x, p.x);
        aabb.lowerBound.y = std::min(aabb.lowerBound.y, p.y);
        aabb.upperBound.x = std::max(aabb.upperBound.x, p.x);
        aabb.upperBound.y = std::max(aabb.upperBound.y, p.y);
    }
    
    calculateCenter();
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
        
        Polygon p{r, material, isDynamic};
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
        
        Polygon p{r, material.type, isDynamic};
        result.push_back(p);
    }
    return result;
}

std::vector<Polygon> Polygon::split(float distance)
{
    std::vector<Polygon> result;
    
    // Make clipping area
    float aabb_left = 0;
    float aabb_right = 0;
    float aabb_top = 0;
    float aabb_bottom = 0;
    jcv_clipping_polygon polygon;
    polygon.num_points = (int)points.size();
    polygon.points = new jcv_point[polygon.num_points];
    for (int i = 0; i < points.size(); i++) {
        float x = (points[i].x + center.x) * UPSCALE;
        float y = (points[i].y + center.y) * UPSCALE;
        polygon.points[i].x = x;
        polygon.points[i].y = y;
        if (i == 0) {
            aabb_left = x;
            aabb_right = x;
            aabb_top = y;
            aabb_bottom = y;
        } else {
            aabb_left = std::min(aabb_left, x);
            aabb_right = std::max(aabb_right, x);
            aabb_top = std::min(aabb_top, y);
            aabb_bottom = std::max(aabb_bottom, y);
        }
    }
    
    jcv_clipper boundsClipper;
    boundsClipper.test_fn = jcv_clip_polygon_test_point;
    boundsClipper.clip_fn = jcv_clip_polygon_clip_edge;
    boundsClipper.fill_fn = jcv_clip_polygon_fill_gaps;
    boundsClipper.ctx = &polygon;
    
    // Create center points for voronoi
    int width = aabb_right - aabb_left;
    int height = aabb_bottom - aabb_top;
    
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    std::default_random_engine engine{(uint8)seed};
    std::uniform_real_distribution<float> distribution{0, 1};
    
    bridson::config conf;
    conf.width = width;
    conf.height = height;
    conf.max_attempts = 10;
    conf.min_distance = distance * UPSCALE;
    
    std::vector<jcv_point> anchorPoints;
    bridson::poisson_disc_distribution(
                                       conf,
                                       // random
                                       [&engine, &distribution](float range)
                                       {
                                           return distribution(engine) * range;
                                       },
                                       // in_area
                                       [&boundsClipper, width, height, aabb_left, aabb_top](bridson::point p)
                                       {
                                           return p.x >= 0 && p.x <= width && p.y >= 0 && p.y <= height;
//                                           jcv_point pt{p.x + aabb_left, p.y + aabb_top};
//                                           return jcv_clip_polygon_test_point(&boundsClipper, pt);
                                       },
                                       // output
                                       [&anchorPoints, aabb_left, aabb_top](bridson::point p)
                                       {
                                           jcv_point pt = jcv_point{p.x + aabb_left, p.y + aabb_top};
                                           anchorPoints.push_back(pt);
                                       }
                                       );
    // Calculate voronoi diagram
    jcv_diagram diagram;
    memset(&diagram, 0, sizeof(jcv_diagram));
    
    jcv_diagram_generate((int)anchorPoints.size(), anchorPoints.data(), 0, /*&boundsClipper*/0, &diagram);
    
    // Create new blocks
    const jcv_site* sites = jcv_diagram_get_sites(&diagram);
    
    for (int i = 0; i < diagram.numsites; i++) {
        const jcv_site* site = &sites[i];
        
        std::vector<b2Vec2> new_points;
        const jcv_graphedge* e = site->edges;
        while(e) {
            jcv_point p = e->pos[0];
            new_points.emplace_back(p.x / UPSCALE, p.y / UPSCALE);
            e = e->next;
        }
        delete e;
        
        if (new_points.size() >= 3) {
            Polygon newPolygon{new_points, material.type, isDynamic};
            
            std::vector<Polygon> clipPolygons;
            clipPolygons.push_back(*this);
            
            auto clipResult = newPolygon.intersection(clipPolygons);
            result.insert(result.end(), clipResult.begin(), clipResult.end());
//            result.push_back(newPolygon);
        }
    }
//    std::vector<Polygon> clippedPolygons = intersection(result);
//    return clippedPolygons;
    return result;
}

std::vector<Polygon> Polygon::splitIntoSubpolygons()
{
    const int threshold = 8;
    
    std::vector<Polygon> result;
    if (points.size() < 3) {
        return result;
    } else if (points.size() <= threshold) {
        result.push_back(*this);
        return result;
    } else {
        p2bayazit::Polygon vertices;
        for (auto pt : points) {
            vertices.push_back(p2bayazit::Point(center.x + pt.x, center.y + pt.y));
        }
        
        p2bayazit::Decomposer dec;
        auto polys = dec.Decompose(vertices);
        
        for (auto subPolygon : polys) {
            std::vector<b2Vec2> subPolygonPoints;
            for (auto vertex : subPolygon) {
                subPolygonPoints.emplace_back(vertex.x, vertex.y);
            }
            Polygon subPolygonResult = Polygon{subPolygonPoints, material.type, isDynamic};
            
            if (subPolygonPoints.size() < 3) {
                continue;
            } else if (subPolygonPoints.size() <= threshold) {
                result.push_back(subPolygonResult);
            } else {
                auto splitResult = subPolygonResult.splitIntoSubpolygons();
                result.insert(result.end(), splitResult.begin(), splitResult.end());
            }
        }
        return result;
    }
}

std::vector<Polygon> Polygon::triangulate()
{
    std::vector<Polygon> result;
    
    int pointCount = (int)points.size();
    
    // Convert points for triangulation
    std::vector<CDT::V2d<float>> delaunayPoints;
    for(int i = 0; i < pointCount; ++i) {
        delaunayPoints.push_back(CDT::V2d<float>{points[i].x + center.x, points[i].y + center.y});
    }
    
    // Remove duplicates
    CDT::RemoveDuplicates(delaunayPoints);
    
    // Triangulate
    CDT::Triangulation<float> cdt;
    cdt.insertVertices(delaunayPoints);
    cdt.eraseSuperTriangle();
    
    // Parse results
    for (auto triangle : cdt.triangles) {
        std::vector<b2Vec2> polygon(3);
        
        auto a = cdt.vertices[triangle.vertices[0]];
        auto b = cdt.vertices[triangle.vertices[1]];
        auto c = cdt.vertices[triangle.vertices[2]];
        
        polygon[0] = b2Vec2(a.x, a.y);
        polygon[1] = b2Vec2(b.x, b.y);
        polygon[2] = b2Vec2(c.x, c.y);
        
        Polygon p{polygon, material.type, isDynamic};
        result.push_back(p);
    }
    return result;
}

Polygon Polygon::makeCircle(b2Vec2 center, float radius, int pointCount, MaterialType materialType, bool isDynamic)
{
    std::vector<b2Vec2> points;
    for (int i = 0; i < pointCount; i++) {
        float angle = M_PI_4 + (2 * M_PI / pointCount) * i;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        points.push_back(b2Vec2(x + center.x, y + center.y));
    }
    
    return Polygon{points, materialType, isDynamic};
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
