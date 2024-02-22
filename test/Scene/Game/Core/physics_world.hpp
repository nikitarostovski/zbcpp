#pragma once
#include "Box2D.h"
#include "base_entity.hpp"
#include "chunk.hpp"
#include <SFML/System.hpp>

class PhysicsWorld : public b2ContactListener
{
private:
    sf::Clock chunkClock;
    b2ParticleSystem *particle_system;
    
    std::map<std::pair<int, int>, Chunk *> chunks;
    
    sf::Vector2i activeChunkX;
    sf::Vector2i activeChunkY;
    
    std::vector<std::pair<int, int>> chunksToDeactivate;
    std::vector<std::pair<int, int>> chunksToActivate;
    
    b2Vec2 lastChunkUpdateCenter;
    
    void processDestroyedEntities();
    void processChunks();
    void processNewLinks();
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
public:
    b2World *world;
    
    PhysicsWorld();
    ~PhysicsWorld();
    
    void updateChunks(b2Vec2 center, float width, float height);
    void step(float dt);
    
    void addEntity(BaseEntity *entity);
    void addLink(BaseEntity *entityA, BaseEntity *entityB);
    
    void render(sf::RenderWindow *window, Camera camera);
};
