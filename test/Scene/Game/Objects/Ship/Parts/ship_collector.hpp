#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "collector_config.hpp"
#include "camera.hpp"
#include "orb.hpp"

class ShipCollector {
protected:
    sf::Color color;
    b2Fixture *gravityFixture;
    sf::CircleShape orbGravityShape;
    
    std::vector<Orb *> orbsToCollect;
    
    void applyGravityToOrb(Orb *orb);
public:
    CollectorConfig config;
    std::function<void(Orb *)> onOrbCollected;
    
    ShipCollector(CollectorConfig config, b2Body *body);
    
    b2Fixture* getFixture();
    void renderFixture(sf::RenderWindow *window, Camera camera);
    
    void receiveCollision(Orb *orb);
    
    void step(float dt);
};
