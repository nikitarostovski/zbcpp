#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D.h"

struct Camera
{
    float x;
    float y;
    float scale;
};

enum CollisionCategory {
    PlayerFrame = 0x0001,
    PlayerEmitter = 0x0002,
    PlayerCollector = 0x0004,
    PlayerShield = 0x0008,
    Asteroid = 0x0010,
    PlanetCore = 0x0020
};

class BaseEntity
{
private:
    std::map<BaseEntity*, bool> collisionMap;
    
protected:
//    b2Body *body;
public:
    bool canBeDestroyed;
    bool isDestroying;
    
    b2Body *body; // TODO: make body protected again
    
    BaseEntity(bool canBeDestroyed);
    virtual ~BaseEntity();
    
    b2Vec2 getPosition();
    
    void destroy();
    virtual void initializeBody(b2World *world) = 0;
    virtual void destroyBody(b2World *world);
    
    virtual void contactBegin(BaseEntity *entity, b2Fixture *fixture);
    virtual void contactEnd(BaseEntity *entity, b2Fixture *fixture);
    void contactSolve(BaseEntity *entity, float impulse);
    
    virtual void receiveCollision(BaseEntity *entity, float impulse) {};
    
    virtual CollisionCategory getEntityType() = 0;
    
    virtual void render(sf::RenderWindow *window, Camera camera) {};
};
