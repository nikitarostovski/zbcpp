#pragma once
#include "Box2D.h"
#include "base_entity.hpp"
#include <SFML/Graphics.hpp>

class PhysicsWorld;
class BodyEntity;
class LiquidEntity;

class GravityFieldEntity : public BaseEntity
{
private:
    PhysicsWorld *world;
    sf::CircleShape shape;
    
    void applyGravityToBody(BodyEntity *entity);
    void applyGravityToLiquid(LiquidEntity *entity);
public:
    b2Vec2 center;
    float radius;
    float strength;
    
    GravityFieldEntity(b2Vec2 pos, float radius, float strength, PhysicsWorld *world);
    
    b2AABB getAABB() override;
    b2Vec2 getPosition() override;
    
    void activate(b2World *world) override;
    void deactivate(b2World *world) override;
    void step(float dt) override;
    void render(sf::RenderWindow *window, Camera camera) override;
};
