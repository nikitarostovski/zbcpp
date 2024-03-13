#pragma once
#include "SFML/Graphics.hpp"
#include "physics_world.hpp"
#include "body_entity.hpp"

class RocketMissle : public BodyEntity
{
private:
    sf::CircleShape shape;
    
protected:
    float radius;
    float damage;
    b2Vec2 impulse;
    
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    RocketMissle(b2Vec2 pos, b2Vec2 impulse, float damage);
    
    void deactivate(b2World *world) override;
    void render(sf::RenderWindow *window, Camera camera) override;
    void contactBegin(BodyEntity *entity, b2Fixture *fixture) override;
    
    std::string getName() override;
};
