#include "fuel_station_building.hpp"
#include "constants.h"

using namespace PolygonUtils;

FuelStationBuilding::FuelStationBuilding(b2Vec2 pos)
    : Building(pos, 3)
    , buildingColor(sf::Color(0, 200, 255))
{ }

b2Body* FuelStationBuilding::initializeBuildingBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_staticBody;
    b2Body *body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape mainShape;
    std::vector<b2Vec2> mainPoints;
    mainPoints.emplace_back(-1, -1);
    mainPoints.emplace_back(-2, 1);
    mainPoints.emplace_back(2, 2);
    mainPoints.emplace_back(2, -2);
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

void FuelStationBuilding::renderBuilding(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();
    
    b2PolygonShape* mainShape = (b2PolygonShape*)mainFixture->GetShape();
    sf::ConvexShape mainPolygon;
    mainPolygon.setFillColor(buildingColor);
    mainPolygon.setOrigin(localCenter);
    mainPolygon.setPosition((worldCenter.x - camera.x) * camera.scale + window->getSize().x / 2,
                            (worldCenter.y - camera.y) * camera.scale + window->getSize().y / 2);
    mainPolygon.setPointCount(mainShape->GetVertexCount());
    for (int i = 0; i < mainShape->GetVertexCount(); i++) {
        mainPolygon.setPoint(i, sf::Vector2<float>(mainShape->GetVertex(i).x * camera.scale, mainShape->GetVertex(i).y * camera.scale));
    }
    mainPolygon.setRotation(angle * DEG_PER_RAD);
    window->draw(mainPolygon);
}
