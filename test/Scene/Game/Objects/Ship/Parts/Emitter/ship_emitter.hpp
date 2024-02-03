#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "base_entity.hpp"
#include "emitter_config.hpp"

using namespace PolygonUtils;

class ShipEmitter : public BaseEntity {
private:
    b2Vec2 pos;
    
public:
    EmitterConfig config;
    ShipEmitter(b2Vec2 pos, EmitterConfig config);
    
    void initializeBody(b2World *world) override;
    CollisionCategory getEntityType() override;
    void render(sf::RenderWindow *window, Camera camera) override;
};
