#pragma once
#include "base_entity.hpp"
#include <SFML/Graphics.hpp>

class BaseEntity;

class GravityField
{
public:
    float x;
    float y;
    float radius;
    float strength;
    
    GravityField(float x, float y, float radius, float strength);
    
    void applyGravityToEntities(std::vector<BaseEntity *> entities);
    void applyGravityToLiquid(b2ParticleSystem *particleSystem);
    
    void render(sf::RenderWindow *window, Camera camera);
};
