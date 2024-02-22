#include "solid_block.hpp"
#include "constants.h"

using namespace PolygonUtils;

SolidBlock::SolidBlock(Polygon polygon)
    : BodyEntity(b2Vec2(polygon.center.x, polygon.center.y), polygon.isDynamic ? Asteroid : PlanetCore, polygon.isDynamic)
    , polygon(polygon)
    , canBeCollected(polygon.isDynamic)
{ }

b2AABB SolidBlock::getInitialAABB()
{
    return polygon.aabb;
}

b2Body* SolidBlock::createBody(b2World *world)
{
    if (polygon.points.size() < 3)
        return nullptr;
    
    b2BodyDef bodyDef;
    bodyDef.userData = static_cast<void*>(this);
    bodyDef.position.Set(polygon.center.x, polygon.center.y);
    bodyDef.type = polygon.isDynamic ? b2_dynamicBody : b2_kinematicBody;
    bodyDef.linearDamping = 0.95f;
    bodyDef.angularDamping = 0.95f;
    b2Body *body = world->CreateBody(&bodyDef);
    
    auto subPolygons = polygon.triangulate();
    for (auto subPolygon : subPolygons) {
        for (int i = 0; i < subPolygon.points.size(); i++) {
            subPolygon.points[i] += subPolygon.center - polygon.center;
        }
        createFixture(body, subPolygon);
    }
    polygon.center = b2Vec2_zero;
    return body;
}

void SolidBlock::receiveCollision(BodyEntity *entity, float impulse)
{
    if (impulse > 10/* && !def.partsDefs.empty()*/) {
//        destroy();
    }
}

void SolidBlock::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2Shape::Type shapeType = f->GetType();
        if (shapeType == b2Shape::e_polygon) {
            b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
            sf::ConvexShape polygonShape;
            
            polygonShape.setOrigin(sf::Vector2<float>(body->GetLocalCenter().x * camera.scale,
                                                      body->GetLocalCenter().y * camera.scale));
            polygonShape.setPosition((body->GetWorldCenter().x - camera.x) * camera.scale + window->getSize().x / 2,
                                     (body->GetWorldCenter().y - camera.y) * camera.scale + window->getSize().y / 2);
            polygonShape.setPointCount(shape->GetVertexCount());
            for (int i = 0; i < shape->GetVertexCount(); i++) {
                polygonShape.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * camera.scale, shape->GetVertex(i).y * camera.scale));
            }
            polygonShape.setRotation(body->GetAngle() * DEG_PER_RAD);
            
            if (canBeCollected) {
                polygonShape.setScale(1.1f, 1.1f);
                polygonShape.setFillColor(sf::Color::Green);
                polygonShape.setOutlineColor(sf::Color::Transparent);
                window->draw(polygonShape);
            }
            
            polygonShape.setScale(1.0f, 1.0f);
            polygonShape.setFillColor(this->polygon.material.color);
            window->draw(polygonShape);
        }
    }
}


//std::vector<SolidBlockDef> SolidBlock::split()
//{
//    std::vector<SolidBlockDef> result;
//    
//    if (def.partsDefs.empty())
//        return result;
//    
//    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
//        b2PolygonShape *shape = (b2PolygonShape *)f->GetShape();
//        std::vector<b2Vec2> points;
//        
//        for (int i = 0; i < shape->GetVertexCount(); i++) {
//            float x = shape->GetVertex(i).x;
//            float y = shape->GetVertex(i).y;
//            b2Vec2 pt = b2Vec2(x, y);
//            pt = body->GetWorldPoint(pt);
//            x = pt.x;
//            y = pt.y;
//            points.emplace_back(x, y);
//        }
//        Polygon p{points, def.polygon.material.type, def.polygon.isDynamic};
//        SolidBlockDef partDef(p, true);
//        auto splitResult = partDef.partsDefs;
//        result.insert(result.end(), splitResult.begin(), splitResult.end());
//    }
//    
//    return result;
//}

void SolidBlock::createFixture(b2Body *body, Polygon polygon)
{
    // Create shape
    b2PolygonShape shape;
    auto points = polygon.points;
    for (int i = 0; i < points.size(); i++)
        points[i] = b2Vec2(points[i].x, points[i].y);
    
    
    // Check duplicates (box2d algorithm)
    bool allPointsUnique = true;
    for (int i = 0; i < polygon.points.size(); i++) {
        for (int j = i + 1; j < polygon.points.size(); j++) {
            if (b2DistanceSquared(polygon.points[i], polygon.points[j]) < 0.5f * b2_linearSlop) {
                allPointsUnique = false;
                goto uniqueExitPoint;
            }
        }
    }
    uniqueExitPoint:
    
    if (!allPointsUnique) {
        return;
    }
    
    shape.Set(points.data(), (int)points.size());

    // Create fixture
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CollisionCategory::Asteroid;
    fixtureDef.filter.maskBits = CollisionCategory::Asteroid | CollisionCategory::PlanetCore | CollisionCategory::PlayerFrame | CollisionCategory::PlayerCollector | CollisionCategory::PlayerEmitter | CollisionCategory::PlayerShield;
    fixtureDef.density = 10;
    fixtureDef.friction = 0.4;
    fixtureDef.restitution = 0;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}

std::string SolidBlock::getName()
{
    return "block";
}
