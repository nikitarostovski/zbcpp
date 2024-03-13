#include "bullet_missle.hpp"
#include "constants.h"

BulletMissle::BulletMissle(b2Vec2 pos, b2Vec2 impulse, float damage)
    : BodyEntity(pos, CategoryOrb)
    , impulse(impulse)
    , damage(damage)
{
    radius = 0.2f;
    shape = sf::CircleShape();
    shape.setFillColor(sf::Color(0, 255, 255));
}

b2AABB BulletMissle::getInitialAABB()
{
    auto pos = getPosition();
    b2AABB aabb;
    aabb.lowerBound.x = pos.x - radius;
    aabb.lowerBound.y = pos.y - radius;
    aabb.upperBound.x = pos.x + radius;
    aabb.upperBound.y = pos.y + radius;
    return aabb;
}

void BulletMissle::deactivate(b2World *world)
{
    isDead = true;
    BodyEntity::deactivate(world);
}

b2Body* BulletMissle::createBody(b2World *world)
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

void BulletMissle::contactBegin(BodyEntity *entity, b2Fixture *fixture) {
    isDead = true;
    entity->receiveDamage(damage);
}

// MARK: - Render

void BulletMissle::render(sf::RenderWindow *window, Camera camera)
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

std::string BulletMissle::getName()
{
    return "bullet";
}
