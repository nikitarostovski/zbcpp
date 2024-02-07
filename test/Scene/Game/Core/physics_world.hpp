#pragma once
#include <vector>
#include "Box2D.h"
#include "gravity_field.hpp"
#include "base_entity.hpp"

class PhysicsWorld : public b2ContactListener
{
private:
//    b2World *world;
    b2ParticleSystem *particle_system;
    
    std::vector<GravityField *> gravityFields;
    std::vector<BaseEntity *> entities;
    
    std::vector<BaseEntity *> entitiesToAdd;
    std::vector<std::pair<BaseEntity *, BaseEntity *>> linksToAdd;
    
    void processGravity();
    void processDestroyedEntities();
    void processNewEntities();
    void processNewLinks();
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
    
public:
    b2World *world;
    
    PhysicsWorld();
    ~PhysicsWorld();
    
    void step(float dt);
    
    void addEntity(BaseEntity *entity);
    void addGravityField(GravityField *field);
    void addLink(BaseEntity *entityA, BaseEntity *entityB);
    void addLiquid(b2Vec2 pos, b2Vec2 velocity, b2ParticleColor color);
    
    void render(sf::RenderWindow *window, Camera camera);
};
