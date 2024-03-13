#include "solid_block.hpp"
#include "constants.h"
#include "orb.hpp"

using namespace PolygonUtils;

SolidBlock::SolidBlock(Polygon polygon, bool isImmortal, PhysicsWorld *world)
    : BodyEntity(b2Vec2(polygon.center.x, polygon.center.y), CategoryTerrain)
    , world(world)
//    , polygon(polygon)
    , material(polygon.material)
    , isImmortal(isImmortal)
{
    initialAABB = polygon.aabb;
    
    b2BodyDef bodyDef;
    bodyDef.position.Set(polygon.center.x, polygon.center.y);
    bodyDef.type = polygon.isDynamic ? b2_dynamicBody : b2_kinematicBody;
    bodyDef.linearDamping = 0.95f;
    bodyDef.angularDamping = 0.95f;
    this->bodyDef = bodyDef;
    this->bodyDef.userData = static_cast<void*>(this);
    
    auto fps = polygon.subPolygons;
    if (fps.empty()) {
        fps.push_back(polygon);
    } else {
        parentPolygonCenterOffset = polygon.center;
    }
    
    for (auto p : fps) {
        b2Vec2 centerShift = p.center - polygon.center;
        
        // Create shape
        b2PolygonShape *shape = new b2PolygonShape();
        auto points = p.points;
        for (int i = 0; i < points.size(); i++) {
            points[i] = b2Vec2(points[i].x, points[i].y) + centerShift;
        }
        
        // Check duplicates (box2d algorithm)
        bool allPointsUnique = true;
        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                if (b2DistanceSquared(points[i], points[j]) < 0.5f * b2_linearSlop) {
                    allPointsUnique = false;
                    goto uniqueExitPoint;
                }
            }
        }
    uniqueExitPoint:
        
        if (!allPointsUnique) {
            return;
        }
        
        shape->Set(points.data(), (int)points.size());
        
        // Create fixture
        b2FixtureDef *fixtureDef = new b2FixtureDef();
        fixtureDef->filter.categoryBits = collisionCategory;
        fixtureDef->filter.maskBits = CategoryShip | CategoryTerrain | CategoryBuilding | CategoryTerrain;
        fixtureDef->density = 10;
        fixtureDef->friction = 0.4;
        fixtureDef->restitution = 0;
        fixtureDef->shape = shape;
        fixtureDefs.push_back(fixtureDef);
        
        fixtureDefPolygons[fixtureDef] = p;
    }
}

b2AABB SolidBlock::getInitialAABB()
{
    return initialAABB;
}

b2Body* SolidBlock::createBody(b2World *world)
{
    b2Body *body = world->CreateBody(&bodyDef);
    
    for (auto fd : fixtureDefs) {
        createFixture(body, fd);
    }
    
    return body;
}

void SolidBlock::createFixture(b2Body *body, b2FixtureDef *def)
{
    auto f = body->CreateFixture(def);
    f->SetUserData(def);
}

void SolidBlock::step(float dt)
{
    if (!body || isDead || fixturesToDestroy.empty())
        return;
    
    for (auto defToDestroy : fixturesToDestroy) {
        for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
            if (defToDestroy == f->GetUserData()) {
                for (int i = 0; i < fixtureDefs.size(); i++) {
                    if (fixtureDefs[i] == defToDestroy) {
                        fixtureDefs.erase(fixtureDefs.begin() + i);
                        fixtureDefPolygons.erase(defToDestroy);
                        break;
                    }
                }
                body->DestroyFixture(f);
                break;
            }
        }
    }
    fixturesToDestroy.clear();
    
    if (!body->GetFixtureList()) {
        isDead = true;
    }
}

// MARK: - Collision

void SolidBlock::receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius)
{
    if (isImmortal || !body)
        return;
    printf("!!! block gets damage  %.2f\n", impulse);
    // TODO: move damage threshold to material
    if (impulse > 2.0f) {
        if (fixtureDefs.size() > 1) {
            spawnSubBlocks(point, radius, true);
        } else {
            spawnOrb(getPosition());
        }
    }
}

void SolidBlock::receiveDamage(float impulse)
{
    receiveCollision(nullptr, impulse, b2Vec2_zero, 0);
}

void SolidBlock::spawnSubBlocks(b2Vec2 point, float radius, bool hasPointAndRadius)
{
    if (!body)
        return;
    
    float radiusSquared = radius * radius;
    
    for (auto fp : fixtureDefPolygons) {
        b2FixtureDef *fixtureDef = fp.first;
        Polygon polygon = fp.second;
        
        for (int i = 0; i < polygon.points.size(); i++) {
            b2Vec2 localPoint = polygon.points[i];
            if (parentPolygonCenterOffset != b2Vec2_zero) {
                localPoint += polygon.center - parentPolygonCenterOffset;
            }
            b2Vec2 p = body->GetWorldPoint(localPoint);
            polygon.points[i] = p - polygon.center;
        }
        
        float distSquared = (polygon.center - point).LengthSquared();
        if (distSquared < radiusSquared) {
            fixturesToDestroy.push_back(fixtureDef);
            polygon.isDynamic = true;
            
            auto nb = new SolidBlock(polygon, isImmortal, world);
            world->addEntity(nb);
        }
    }
}

void SolidBlock::spawnOrb(b2Vec2 point)
{
    isDead = true;
    auto orb = new Orb(point);
    world->addEntity(orb);
}

// MARK: - Render

void SolidBlock::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    for (auto fp : fixtureDefPolygons) {
        Polygon polygon = fp.second;
        
        sf::Vertex vStart, vLast;
        for (int i = 0; i < polygon.points.size(); i++) {
            b2Vec2 localPoint = polygon.points[i];
            if (parentPolygonCenterOffset != b2Vec2_zero) {
                localPoint += polygon.center - parentPolygonCenterOffset;
            }
            b2Vec2 p = body->GetWorldPoint(localPoint);
            
            sf::Vertex v;
            v.position.x = p.x;
            v.position.y = p.y;
            v.texCoords.x = polygon.texCoords[i].x;
            v.texCoords.y = polygon.texCoords[i].y;
            
            if (i == 0) {
                vStart = v;
                continue;
            }
            if (i == 1) {
                vLast = v;
                continue;
            }
            
            world->terrainRenderers[material.type]->addVertex(vStart);
            world->terrainRenderers[material.type]->addVertex(vLast);
            world->terrainRenderers[material.type]->addVertex(v);
            
            vLast = v;
        }
    }
}

std::string SolidBlock::getName()
{
    return "block";
}
