#pragma once
#include "physics_world.hpp"
#include "base_entity.hpp"

class AsteroidBelt {
private:
    
public:
    AsteroidBelt(b2Vec2 pos, float startRadius, float endRadius, float asteroidSize, PhysicsWorld *world);
    ~AsteroidBelt();
    
    void step(float dt);
};
