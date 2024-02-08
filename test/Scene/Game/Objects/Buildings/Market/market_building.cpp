#include "market_building.hpp"
#include "constants.h"

using namespace PolygonUtils;

MarketBuilding::MarketBuilding(b2Vec2 pos)
    : Building(pos, 70)
    , buildingColor(sf::Color(255, 0, 255))
{ }

CollisionCategory MarketBuilding::getEntityType()
{
    return CollisionCategory::PlanetCore;
}

b2Body* MarketBuilding::initializeBuildingBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x / PPM, pos.y / PPM);
    bodyDef.type = b2_staticBody;
    b2Body *body = world->CreateBody(&bodyDef);
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
    
    return body;
}

void MarketBuilding::renderBuilding(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();
    
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
