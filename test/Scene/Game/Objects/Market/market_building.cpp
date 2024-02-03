#include "market_building.hpp"
#include "constants.h"

using namespace PolygonUtils;

MarketBuilding::MarketBuilding(b2Vec2 pos)
    : BaseEntity(false)
    , pos(pos)
    , buildingColor(sf::Color::Cyan)
    , contactAreaColor(sf::Color(100, 100, 100, 64))
{ }

CollisionCategory MarketBuilding::getEntityType()
{
    return CollisionCategory::PlanetCore;
}

void MarketBuilding::initializeBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x / PPM, pos.y / PPM);
    bodyDef.type = b2_staticBody;
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape mainShape;
    std::vector<b2Vec2> mainPoints;
    mainPoints.emplace_back(-20 / PPM, -20 / PPM);
    mainPoints.emplace_back(-20 / PPM, 20 / PPM);
    mainPoints.emplace_back(20 / PPM, 20 / PPM);
    mainPoints.emplace_back(20 / PPM, -20 / PPM);
    mainShape.Set(mainPoints.data(), (int)mainPoints.size());

    b2FixtureDef mainFixtureDef;
    mainFixtureDef.filter.categoryBits = PlanetCore;
    mainFixtureDef.filter.maskBits = Asteroid | PlanetCore;
    mainFixtureDef.density = 0.5;
    mainFixtureDef.friction = 0.5;
    mainFixtureDef.restitution = 0.2;
    mainFixtureDef.shape = &mainShape;
    
    mainFixture = body->CreateFixture(&mainFixtureDef);
    
    
    b2CircleShape contactShape;
    contactShape.m_radius = 70 / PPM;

    b2FixtureDef contactFixtureDef;
    contactFixtureDef.isSensor = true;
    contactFixtureDef.filter.categoryBits = PlanetCore;
    contactFixtureDef.filter.maskBits = PlayerFrame;
    contactFixtureDef.shape = &contactShape;

    contactFixture = body->CreateFixture(&contactFixtureDef);
}

void MarketBuilding::contactBegin(BaseEntity *entity, b2Fixture *fixture)
{
    if (entity->getEntityType() != PlayerFrame)
        return;
    contactAreaColor = sf::Color(0, 255, 0, 64);
    if (onPlayerEnter)
        onPlayerEnter();
}

void MarketBuilding::contactEnd(BaseEntity *entity, b2Fixture *fixture)
{
    if (entity->getEntityType() != PlayerFrame)
        return;
    contactAreaColor = sf::Color(sf::Color(100, 100, 100, 64));
    if (onPlayerLeave)
        onPlayerLeave();
}

void MarketBuilding::render(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();
    
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
    
    // Building
    b2PolygonShape* mainShape = (b2PolygonShape*)mainFixture->GetShape();
    sf::ConvexShape mainPolygon;
    mainPolygon.setFillColor(buildingColor);
    mainPolygon.setOrigin(localCenter);
    mainPolygon.setPosition((worldCenter.x - camera.x) * PPM + window->getSize().x / 2,
                            (worldCenter.y - camera.y) * PPM + window->getSize().y / 2);
    mainPolygon.setPointCount(mainShape->GetVertexCount());
    for (int i = 0; i < mainShape->GetVertexCount(); i++) {
        mainPolygon.setPoint(i, sf::Vector2<float>(mainShape->GetVertex(i).x * PPM, mainShape->GetVertex(i).y * PPM));
    }
    mainPolygon.setRotation(angle * DEG_PER_RAD);
    window->draw(mainPolygon);
}
