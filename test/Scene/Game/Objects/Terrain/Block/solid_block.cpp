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
    renderShape = sf::ConvexShape();
    renderShape.setFillColor(polygon.material.color);
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
    
    if (!polygon.subPolygons.empty()) {
        for (auto p : polygon.subPolygons) {
            createFixture(body, p);
        }
    } else {
        createFixture(body, polygon);
    }
    
    return body;
}

void SolidBlock::createFixture(b2Body *body, Polygon polygon)
{
    b2Vec2 centerShift = polygon.center - this->polygon.center;
    
    // Create shape
    b2PolygonShape shape;
    auto points = polygon.points;
    for (int i = 0; i < points.size(); i++) {
        points[i] = b2Vec2(points[i].x, points[i].y) + centerShift;
        cachedVertices.push_back({{0, 0}, polygon.material.color});
    }
    // for center point
    cachedVertices.push_back({{0, 0}, polygon.material.color});
    
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
    
    auto renderShape = sf::ConvexShape();
    renderShape.setFillColor(polygon.material.color);
    renderShape.setOutlineColor(sf::Color::White);
    renderShape.setOutlineThickness(1.0f);
    renderShapes.push_back(renderShape);
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
    if (isImmortal)
        return;
    
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
    float radiusSquared = radius * radius;
    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2Shape::Type shapeType = f->GetType();
        if (shapeType == b2Shape::e_polygon) {
            b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
            
            std::vector<b2Vec2> newPoints;
            for (int i = 0; i < shape->GetVertexCount(); i++) {
                newPoints.push_back(shape->GetVertex(i) + polygon.center);
            }
            Polygon newP = Polygon(newPoints, polygon.material.type, polygon.isDynamic, false);
            
            float distSquared = (shape->m_centroid + polygon.center - point).LengthSquared();
            if (distSquared < radiusSquared) {
                newP.isDynamic = true;
                newP.material = Material(MaterialType::red);
            }
            auto nb = new SolidBlock(newP, isImmortal, world);
            world->addEntity(nb);
        }
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
    
    int vi = 0;
    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2Shape::Type shapeType = f->GetType();
        if (shapeType == b2Shape::e_polygon) {
            b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
            
            float cx = body->GetWorldCenter().x;
            float cy = body->GetWorldCenter().y;
            
            // TODO: not copy all vertices, but resize vertex array in one go
            int ci = vi + shape->GetVertexCount() - 1;
            auto vc = cachedVertices[ci];
            vc.position.x = cx;
            vc.position.y = cy;
            
            b2Vec2 pl = body->GetWorldPoint(shape->GetVertex(shape->GetVertexCount() - 1));
            sf::Vertex last = cachedVertices[vi + shape->GetVertexCount() - 1];
            last.position.x = pl.x;
            last.position.y = pl.y;
            
            for (int i = 0; i < shape->GetVertexCount(); i++) {
                b2Vec2 p = body->GetWorldPoint(shape->GetVertex(i));
                
                auto v1 = cachedVertices[vi + i];
                v1.position.x = p.x;
                v1.position.y = p.y;
                
                world->terrainRenderer->addVertex(v1);
                world->terrainRenderer->addVertex(last);
                world->terrainRenderer->addVertex(vc);
                
                last = v1;
            }
            vi += shape->GetVertexCount();
        }
    }
}

std::string SolidBlock::getName()
{
    return "block";
}
