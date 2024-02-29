#pragma once
#include "SFML/Graphics.hpp"
#include "physics_world.hpp"
#include "body_entity.hpp"

class Orb : public BodyEntity
{
private:
    sf::CircleShape shape;
    
protected:
    float radius;
    
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    Orb(b2Vec2 pos);
    
    void render(sf::RenderWindow *window, Camera camera) override;
    void receiveDamage(float impulse) override;
    
    std::string getName() override;
};
