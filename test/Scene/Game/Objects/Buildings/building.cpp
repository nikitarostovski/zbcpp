#include "building.hpp"
#include "constants.h"

using namespace PolygonUtils;

Building::Building(b2Vec2 pos, float contactRadius)
    : BodyEntity(pos, CategoryBuilding)
    , pos(pos)
    , contactRadius(contactRadius)
    , contactAreaColor(sf::Color(100, 100, 100, 64))
{ }

b2AABB Building::getInitialAABB()
{
    b2AABB result;
    result.lowerBound.x = pos.x - contactRadius;
    result.lowerBound.y = pos.y - contactRadius;
    result.upperBound.x = pos.x + contactRadius;
    result.upperBound.y = pos.y + contactRadius;
    return result;
}

b2Body* Building::createBody(b2World *world)
{
    b2Body *body = initializeBuildingBody(world);
    
    b2CircleShape contactShape;
    contactShape.m_radius = contactRadius;

    b2FixtureDef contactFixtureDef;
    contactFixtureDef.isSensor = true;
    contactFixtureDef.filter.categoryBits = CategoryBuilding;
    contactFixtureDef.filter.maskBits = CategoryShip;
    contactFixtureDef.shape = &contactShape;

    contactFixture = body->CreateFixture(&contactFixtureDef);
    return body;
}

void Building::contactBegin(BodyEntity *entity, b2Fixture *fixture)
{
    if (entity->collisionCategory != CategoryShip)
        return;
    contactAreaColor = sf::Color(0, 255, 0, 64);
    if (onPlayerEnter)
        onPlayerEnter();
}

void Building::contactEnd(BodyEntity *entity, b2Fixture *fixture)
{
    if (entity->collisionCategory != CategoryShip)
        return;
    contactAreaColor = sf::Color(sf::Color(100, 100, 100, 64));
    if (onPlayerLeave)
        onPlayerLeave();
}

void Building::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    
    // Contact area
    b2CircleShape* contactShape = (b2CircleShape*)contactFixture->GetShape();
    float contactRadius = contactShape->m_radius * camera.scale;
    sf::CircleShape contactCircle;
    contactCircle.setFillColor(contactAreaColor);
    contactCircle.setOrigin(localCenter);
    contactCircle.setPosition((worldCenter.x - camera.x) * camera.scale + window->getSize().x / 2 - contactRadius,
                              (worldCenter.y - camera.y) * camera.scale + window->getSize().y / 2 - contactRadius);
    contactCircle.setRadius(contactShape->m_radius * camera.scale);
    window->draw(contactCircle);
    
    renderBuilding(window, camera);
}
