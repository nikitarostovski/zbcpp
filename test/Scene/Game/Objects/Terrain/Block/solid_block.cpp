#include "solid_block.hpp"
#include "constants.h"
#include "orb.hpp"

using namespace PolygonUtils;

SolidBlock::SolidBlock(Polygon polygon, bool isImmortal, PhysicsWorld *world)
    : BodyEntity(b2Vec2(polygon.center.x, polygon.center.y), CategoryTerrain)
    , world(world)
    , polygon(polygon)
    , isImmortal(isImmortal)
{
//    if (!polygon.subPolygons.empty()) {
//        isDead = true;
//        for (auto p: polygon.subPolygons) {
//            auto nb = new SolidBlock(p, isImmortal, world);
//            world->addEntity(nb);
//        }
//    }
}

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
    fixtureDef.filter.categoryBits = collisionCategory;
    fixtureDef.filter.maskBits = CategoryShip | CategoryTerrain | CategoryBuilding | CategoryTerrain;
    fixtureDef.density = 10;
    fixtureDef.friction = 0.4;
    fixtureDef.restitution = 0;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}

// MARK: - Collision

void SolidBlock::receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius)
{
    if (isImmortal)
        return;
    
    if (impulse > 100) {
        isDead = true;
        if (!polygon.subPolygons.empty()) {
            spawnSubBlocks(point, radius, true);
        } else {
            spawnOrb(getPosition());
        }
    }
}

void SolidBlock::receiveDamage(float impulse)
{
    if (impulse > 100) {
        isDead = true;
        if (!polygon.subPolygons.empty()) {
            spawnSubBlocks(b2Vec2_zero, 0, false);
        } else {
            spawnOrb(getPosition());
        }
    }
}

void SolidBlock::spawnSubBlocks(b2Vec2 point, float radius, bool hasPointAndRadius)
{
    for (auto p : polygon.subPolygons) {
        if (hasPointAndRadius) {
            float dist = (p.center - point).Length();
            if (dist > radius) {
                p.isDynamic = polygon.isDynamic;
            } else {
                p.isDynamic = true;
                p.material = Material(MaterialType::red);
            }
        }
        auto nb = new SolidBlock(p, isImmortal, world);
        world->addEntity(nb);
    }
}

void SolidBlock::spawnOrb(b2Vec2 point)
{
    auto orb = new Orb(point);
    world->addEntity(orb);
}

// MARK: - Render

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
            
//            polygonShape.setScale(1.1f, 1.1f);
//            polygonShape.setFillColor(sf::Color::Green);
//            polygonShape.setOutlineColor(sf::Color::Transparent);
//            window->draw(polygonShape);
            
            polygonShape.setScale(1.0f, 1.0f);
            polygonShape.setFillColor(this->polygon.material.color);
            window->draw(polygonShape);
        }
    }
}

std::string SolidBlock::getName()
{
    return "block";
}
