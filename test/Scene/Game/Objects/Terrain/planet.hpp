#pragma once
#include "physics_world.hpp"
#include "base_entity.hpp"

class Planet {
private:
    
public:
    b2Vec2 center;
    float gravityRadius;
    
    Planet(b2Vec2 pos, float radius, float gravityRadius, PhysicsWorld *world);
    ~Planet();
    
    void step(float dt);
};
