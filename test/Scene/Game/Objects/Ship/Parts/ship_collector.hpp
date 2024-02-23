#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "collector_config.hpp"
#include "camera.hpp"

class ShipCollector {
protected:
    sf::Color color;
    b2Fixture *fixture;
public:
    CollectorConfig config;
    
    ShipCollector(CollectorConfig config, b2Body *body);
    
    b2Fixture* getFixture();
    void renderFixture(sf::RenderWindow *window, Camera camera);
};
