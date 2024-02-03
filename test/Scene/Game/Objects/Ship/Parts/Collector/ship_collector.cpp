#include "ship_collector.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipCollector::ShipCollector(b2Vec2 pos, CollectorConfig config)
    : BaseEntity(false)
    , config(config)
    , pos(pos)
{
    
}

void ShipCollector::initializeBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x / PPM, pos.y / PPM);
    bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> points;
    points.emplace_back(10 / PPM, -10 / PPM);
    points.emplace_back(-10 / PPM, -10 / PPM);
    points.emplace_back(-15 / PPM, 0);
    points.emplace_back(15 / PPM, 0);
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
}

CollisionCategory ShipCollector::getEntityType()
{
    return CollisionCategory::PlayerCollector;
}

void ShipCollector::contactBegin(BaseEntity *entity, b2Fixture *fixture)
{
    if (!entity)
        return;
    
    if (entity->getEntityType() == CollisionCategory::Asteroid) {
        entity->destroy();
        if (onMaterialCollect)
            onMaterialCollect(1);
    }
}

void ShipCollector::render(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2Fixture *fixture = body->GetFixtureList();
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(sf::Color(32, 255, 255, 128));
    polygon.setOrigin(localCenter);
    polygon.setPosition((worldCenter.x - camera.x) * PPM + window->getSize().x / 2,
                        (worldCenter.y - camera.y) * PPM + window->getSize().y / 2);
    polygon.setPointCount(shape->GetVertexCount());
    for (int i = 0; i < shape->GetVertexCount(); i++) {
        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * PPM, shape->GetVertex(i).y * PPM));
    }
    polygon.setRotation(angle * DEG_PER_RAD);
    window->draw(polygon);
}
