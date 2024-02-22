#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "body_entity.hpp"
#include "frame_config.hpp"

using namespace PolygonUtils;

class ShipFrame : public BodyEntity {
protected:
    b2Vec2 pos;
    sf::Color color;
    
public:
    FrameConfig config;
    std::function<void(float)> onDamageReceive;
    
    ShipFrame(b2Vec2 pos, FrameConfig config, sf::Color color);
    
    void render(sf::RenderWindow *window, Camera camera) override;
};
