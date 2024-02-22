#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "building.hpp"

using namespace PolygonUtils;

class FuelStationBuilding : public Building {
private:
    b2Fixture *mainFixture;
protected:
    sf::Color buildingColor;
public:
    FuelStationBuilding(b2Vec2 pos);
    
    b2Body* initializeBuildingBody(b2World *world) override;
    void renderBuilding(sf::RenderWindow *window, Camera camera) override;
};
