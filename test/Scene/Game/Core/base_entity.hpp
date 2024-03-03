#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D.h"
#include "camera.hpp"

class BaseEntity
{
public:
    int lastChunkXLo;
    int lastChunkXHi;
    int lastChunkYLo;
    int lastChunkYHi;
    
    bool isDead;
    
    BaseEntity();
    virtual ~BaseEntity();
    
    virtual std::string getName();
    
    bool checkIfChunkChanged();
    
    virtual b2AABB getAABB() = 0;
    virtual b2Vec2 getPosition() = 0;
    
    virtual void activate(b2World *world) = 0;
    virtual void deactivate(b2World *world) = 0;
    
    virtual void step(float dt) {};
    virtual void render(sf::RenderWindow *window, Camera camera) {};
};
