#include "building.hpp"
#include "constants.h"

using namespace PolygonUtils;

Building::Building(b2Vec2 pos, float contactRadius)
    : BaseEntity(false)
    , pos(pos)
    , contactRadius(contactRadius)
    , contactAreaColor(sf::Color(100, 100, 100, 64))
{ }

void Building::initializeBody(b2World *world)
{
    body = initializeBuildingBody(world);
    
    b2CircleShape contactShape;
    contactShape.m_radius = contactRadius / PPM;

    b2FixtureDef contactFixtureDef;
    contactFixtureDef.isSensor = true;
    contactFixtureDef.filter.categoryBits = PlanetCore;
    contactFixtureDef.filter.maskBits = PlayerFrame;
    contactFixtureDef.shape = &contactShape;

    contactFixture = body->CreateFixture(&contactFixtureDef);
}

void Building::contactBegin(BaseEntity *entity, b2Fixture *fixture)
{
    if (entity->getEntityType() != PlayerFrame)
        return;
    contactAreaColor = sf::Color(0, 255, 0, 64);
    if (onPlayerEnter)
        onPlayerEnter();
}

void Building::contactEnd(BaseEntity *entity, b2Fixture *fixture)
{
    if (entity->getEntityType() != PlayerFrame)
        return;
    contactAreaColor = sf::Color(sf::Color(100, 100, 100, 64));
    if (onPlayerLeave)
        onPlayerLeave();
}

void Building::render(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    
    // Contact area
    b2CircleShape* contactShape = (b2CircleShape*)contactFixture->GetShape();
    float contactRadius = contactShape->m_radius * PPM;
    sf::CircleShape contactCircle;
    contactCircle.setFillColor(contactAreaColor);
    contactCircle.setOrigin(localCenter);
    contactCircle.setPosition((worldCenter.x - camera.x) * PPM + window->getSize().x / 2 - contactRadius,
                              (worldCenter.y - camera.y) * PPM + window->getSize().y / 2 - contactRadius);
    contactCircle.setRadius(contactShape->m_radius * PPM);
    window->draw(contactCircle);
    
    renderBuilding(window, camera);
}
