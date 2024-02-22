#include "ship_emitter.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipEmitter::ShipEmitter(b2Vec2 pos, EmitterConfig config)
    : BodyEntity(pos, PlayerEmitter, false)
    , config(config)
    , pos(pos)
{ }

b2AABB ShipEmitter::getInitialAABB()
{
    b2AABB result;
    result.lowerBound.x = pos.x - 5;
    result.lowerBound.y = pos.y + 5;
    result.upperBound.x = pos.x + 5;
    result.upperBound.y = pos.y + 5.5;
    return result;
}

b2Body* ShipEmitter::createBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world->CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> points;
    points.emplace_back(-1.5, 5);
    points.emplace_back(1.5, 5);
    points.emplace_back(5, 5.5);
    points.emplace_back(-5, 5.5);
    shape.Set(points.data(), (int)points.size());
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CollisionCategory::PlayerEmitter;
    fixtureDef.filter.maskBits = 0;//CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
    fixtureDef.density = 0.5;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;
    
    body->CreateFixture(&fixtureDef);
    return body;
}

void ShipEmitter::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2Fixture *fixture = body->GetFixtureList();
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(sf::Color::Cyan);
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
