#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "body_entity.hpp"
#include "emitter_config.hpp"

using namespace PolygonUtils;

class ShipEmitter : public BodyEntity {
private:
    b2Vec2 pos;
    
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    EmitterConfig config;
    
    ShipEmitter(b2Vec2 pos, EmitterConfig config);
    
    void render(sf::RenderWindow *window, Camera camera) override;
};
