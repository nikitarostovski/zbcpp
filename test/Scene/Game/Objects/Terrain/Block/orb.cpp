#include "orb.hpp"
#include "constants.h"

Orb::Orb(b2Vec2 pos)
    : BodyEntity(pos, CategoryOrb)
{ 
    radius = 0.5f;
    shape = sf::CircleShape();
    shape.setFillColor(sf::Color(255, 0, 255));
}

b2AABB Orb::getInitialAABB()
{
    auto pos = getPosition();
    b2AABB aabb;
    aabb.lowerBound.x = pos.x - radius;
    aabb.lowerBound.y = pos.y - radius;
    aabb.upperBound.x = pos.x + radius;
    aabb.upperBound.y = pos.y + radius;
    return aabb;
}

b2Body* Orb::createBody(b2World *world)
{
    auto pos = getPosition();
    
    b2BodyDef bodyDef;
    bodyDef.userData = static_cast<void*>(this);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 0.95f;
    bodyDef.angularDamping = 0.95f;
    b2Body *body = world->CreateBody(&bodyDef);
    
    
    b2CircleShape shape;
    shape.m_radius = radius;
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CategoryOrb;
    fixtureDef.filter.maskBits = CategoryShipCollector;
    fixtureDef.isSensor = true;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    
    return body;
}

void Orb::receiveDamage(float impulse) {}

// MARK: - Render

void Orb::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    shape.setRadius(radius * camera.scale);
    shape.setOrigin(sf::Vector2<float>((body->GetLocalCenter().x + radius) * camera.scale,
                                       (body->GetLocalCenter().y + radius) * camera.scale));
    shape.setPosition((body->GetWorldCenter().x - camera.x) * camera.scale + window->getSize().x / 2,
                      (body->GetWorldCenter().y - camera.y) * camera.scale + window->getSize().y / 2);
    window->draw(shape);
}

std::string Orb::getName()
{
    return "orb";
}
