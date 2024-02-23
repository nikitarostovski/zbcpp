#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "emitter_config.hpp"
#include "camera.hpp"

class ShipEmitter {
protected:
    sf::Color color;
    b2Fixture *fixture;
public:
    EmitterConfig config;
    
    ShipEmitter(EmitterConfig config, b2Body *body);
    
    b2Fixture* getFixture();
    void renderFixture(sf::RenderWindow *window, Camera camera);
};
