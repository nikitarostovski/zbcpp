#include "rocket_missle.hpp"
#include "constants.h"

RocketMissle::RocketMissle(b2Vec2 pos, b2Vec2 impulse, float damage)
    : BodyEntity(pos, CategoryOrb)
    , impulse(impulse)
    , damage(damage)
{
    radius = 0.5f;
    shape = sf::CircleShape();
    shape.setFillColor(sf::Color(255, 0, 0));
}

b2AABB RocketMissle::getInitialAABB()
{
    auto pos = getPosition();
    b2AABB aabb;
    aabb.lowerBound.x = pos.x - radius;
    aabb.lowerBound.y = pos.y - radius;
    aabb.upperBound.x = pos.x + radius;
    aabb.upperBound.y = pos.y + radius;
    return aabb;
}

void RocketMissle::deactivate(b2World *world)
{
    isDead = true;
    BodyEntity::deactivate(world);
}

b2Body* RocketMissle::createBody(b2World *world)
{
    auto pos = getPosition();
    
    b2BodyDef bodyDef;
    bodyDef.userData = static_cast<void*>(this);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 0;
    bodyDef.bullet = true;
    b2Body *body = world->CreateBody(&bodyDef);
    
    
    b2CircleShape shape;
    shape.m_radius = radius;
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CategoryOrb;
    fixtureDef.filter.maskBits = CategoryTerrain;
    fixtureDef.isSensor = true;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
    
    return body;
}

void RocketMissle::contactBegin(BodyEntity *entity, b2Fixture *fixture) {
    isDead = true;
    // TODO: explosion
    entity->receiveDamage(damage);
}

// MARK: - Render

void RocketMissle::render(sf::RenderWindow *window, Camera camera)
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

std::string RocketMissle::getName()
{
    return "rocket";
}
