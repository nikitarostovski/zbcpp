#include "ship_collector.hpp"
#include "constants.h"
#include "collision_category.hpp"

ShipCollector::ShipCollector(CollectorConfig config, b2Body *body)
    : config(config)
    , color(sf::Color(200, 140, 20))
{
    b2CircleShape gravityShape;
    gravityShape.m_radius = config.orbCollectionRadius;

    b2FixtureDef gravityFixtureDef;
    gravityFixtureDef.filter.categoryBits = CategoryShipCollector;
    gravityFixtureDef.filter.maskBits = CategoryOrb;
    gravityFixtureDef.isSensor = true;
    gravityFixtureDef.shape = &gravityShape;
    gravityFixture = body->CreateFixture(&gravityFixtureDef);
    
    orbGravityShape = sf::CircleShape();
    orbGravityShape.setFillColor(sf::Color(100, 100, 100, 32));
}

b2Fixture* ShipCollector::getFixture()
{
    return gravityFixture;
}

void ShipCollector::receiveCollision(Orb *orb)
{
    for (auto o : orbsToCollect) {
        if (o == orb)
            return;
    }
    orbsToCollect.push_back(orb);
}

void ShipCollector::step(float dt)
{
    const float collectionRadius = 1.0f;
    if (!gravityFixture)
        return;
    for (int i = 0; i < orbsToCollect.size();) {
        auto orb = orbsToCollect[i];
        applyGravityToOrb(orb);
        
        auto dist = (orb->getPosition() - gravityFixture->GetBody()->GetPosition()).Length();
        bool isCollected = dist <= collectionRadius;
        if (isCollected) {
            onOrbCollected(orb);
            orbsToCollect.erase(orbsToCollect.begin() + i);
            orb->isDead = true;
        } else {
            i++;
        }
    }
}

void ShipCollector::applyGravityToOrb(Orb *orb)
{
    const float strength = 5.0f;
    
    b2Vec2 fieldCenter = gravityFixture->GetBody()->GetPosition();
    b2Body *body = orb->body;
    if (!body)
        return;
    
    b2Vec2 center = body->GetPosition();
    b2Vec2 shift = center - fieldCenter;
    
    b2Vec2 imp = -shift / shift.Normalize() * strength / shift.Length();
    body->ApplyLinearImpulse(imp, body->GetWorldCenter(), true);
}

void ShipCollector::renderFixture(sf::RenderWindow *window, Camera camera)
{
    if (!gravityFixture)
        return;
    
    b2Body *body = gravityFixture->GetBody();
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    
    orbGravityShape.setRadius(config.orbCollectionRadius * camera.scale);
    orbGravityShape.setOrigin(localCenter);
    orbGravityShape.setPosition((worldCenter.x - config.orbCollectionRadius - camera.x) * camera.scale + window->getSize().x / 2,
                                (worldCenter.y - config.orbCollectionRadius - camera.y) * camera.scale + window->getSize().y / 2);
    window->draw(orbGravityShape);
}
