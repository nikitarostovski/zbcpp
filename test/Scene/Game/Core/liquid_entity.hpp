#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D.h"
#include "base_entity.hpp"

class LiquidEntity : public BaseEntity
{
private:
    sf::Clock clock;
    sf::CircleShape shape;
    
    b2ParticleColor getParticleColor();
protected:
    b2ParticleDef particleDef;
public:
    const b2ParticleHandle *handle;
    b2ParticleSystem *system;
    
    LiquidEntity(b2Vec2 initialPosition, b2Vec2 velocity, b2ParticleColor color);
    ~LiquidEntity();
    
    void activate(b2World *world) override;
    void deactivate(b2World *world) override;
    
    b2AABB getAABB() override;
    b2Vec2 getPosition() override;
    
    void render(sf::RenderWindow *window, Camera camera) override;
    
    std::string getName() override;
};
