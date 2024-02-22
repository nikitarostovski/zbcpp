#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D.h"
#include "base_entity.hpp"
#include "polygon.hpp"

enum CollisionCategory {
    PlayerFrame = 0x0001,
    PlayerEmitter = 0x0002,
    PlayerCollector = 0x0004,
    PlayerShield = 0x0008,
    Asteroid = 0x0010,
    PlanetCore = 0x0020
};

class BodyEntity : public BaseEntity
{
private:
    bool didSetupAABB;
    std::map<BaseEntity*, bool> collisionMap;
    b2Vec2 lastKnownPosition;
    float lastKnownAngle;
    b2AABB lastKnownAABB;
protected:
    virtual b2Body* createBody(b2World *world) = 0;
    virtual b2AABB getInitialAABB() = 0;
public:
    bool canBeDestroyed;
    bool isDestroying;
    CollisionCategory collisionCategory;
    b2Body *body;
    
    BodyEntity(b2Vec2 initialPosition, CollisionCategory collisionCategory, bool canBeDestroyed);
    ~BodyEntity();
    
    void activate(b2World *world) override;
    void deactivate(b2World *world) override;
    
    b2AABB getAABB() override;
    b2Vec2 getPosition() override;
    
    virtual void contactBegin(BodyEntity *entity, b2Fixture *fixture);
    virtual void contactEnd(BodyEntity *entity, b2Fixture *fixture);
    void contactSolve(BodyEntity *entity, float impulse);
    
    virtual void receiveCollision(BodyEntity *entity, float impulse) {};
};
