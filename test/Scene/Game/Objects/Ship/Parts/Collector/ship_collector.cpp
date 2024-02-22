#include "ship_collector.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipCollector::ShipCollector(b2Vec2 pos, CollectorConfig config)
    : BodyEntity(pos, PlayerCollector, false)
    , config(config)
    , pos(pos)
{ }

b2AABB ShipCollector::getInitialAABB()
{
    b2AABB result;
    result.lowerBound.x = pos.x - 1.5;
    result.lowerBound.y = pos.y -1;
    result.upperBound.x = pos.x + 1.5;
    result.upperBound.y = pos.y;
    return result;
}

b2Body* ShipCollector::createBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> points;
    points.emplace_back(1, -1);
    points.emplace_back(-1, -1);
    points.emplace_back(-1.5, 0);
    points.emplace_back(1.5, 0);
    shape.Set(points.data(), (int)points.size());
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CollisionCategory::PlayerCollector;
    fixtureDef.filter.maskBits = CollisionCategory::Asteroid;
    fixtureDef.isSensor = true;
    fixtureDef.density = 0.5;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;
    
    body->CreateFixture(&fixtureDef);
    return body;
}

void ShipCollector::contactBegin(BodyEntity *entity, b2Fixture *fixture)
{
    if (!entity)
        return;
    
    if (entity->collisionCategory == Asteroid) {
//        entity->destroy();
        if (onMaterialCollect)
            onMaterialCollect(1);
    }
}

void ShipCollector::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2Fixture *fixture = body->GetFixtureList();
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(sf::Color(32, 255, 255, 128));
    polygon.setOrigin(localCenter);
    polygon.setPosition((worldCenter.x - camera.x) * camera.scale + window->getSize().x / 2,
                        (worldCenter.y - camera.y) * camera.scale + window->getSize().y / 2);
    polygon.setPointCount(shape->GetVertexCount());
    for (int i = 0; i < shape->GetVertexCount(); i++) {
        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * camera.scale, shape->GetVertex(i).y * camera.scale));
    }
    polygon.setRotation(angle * DEG_PER_RAD);
    window->draw(polygon);
}
