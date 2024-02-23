#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "frame_config.hpp"
#include "camera.hpp"

class ShipFrame {
protected:
    sf::Color color;
    b2Fixture *fixture;
public:
    FrameConfig config;
    
    ShipFrame(FrameConfig config, b2Body *body);
    
    b2Fixture* getFixture();
    void renderFixture(sf::RenderWindow *window, Camera camera);
};
