#pragma once
#include "Box2D.h"
#include "base_entity.hpp"
#include "chunk.hpp"
#include <SFML/System.hpp>

#include "terrain_renderer.hpp"

class PhysicsWorld : public b2ContactListener
{
private:
    sf::Clock chunkClock;
    b2ParticleSystem *particle_system;
    
    std::map<std::pair<BaseEntity *, BaseEntity *>, bool> disabledContacts;
    
    std::map<std::pair<int, int>, Chunk *> chunks;
    
    sf::Vector2i activeChunkX;
    sf::Vector2i activeChunkY;
    
    std::vector<std::pair<int, int>> chunksToDeactivate;
    std::vector<std::pair<int, int>> chunksToActivate;
    
    std::vector<BaseEntity *> entitiesToActivate;
    
    b2Vec2 lastChunkUpdateCenter;
    
    void processDestroyedEntities();
    void processChunks();
    void processNewEntities();
    void updateEntitesChunks();
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {};
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
public:
    b2World *world;
    
    TerrainRenderer *terrainRenderer;
    
    PhysicsWorld();
    ~PhysicsWorld();
    
    void updateChunks(b2Vec2 center, float width, float height);
    void step(float dt);
    
    Chunk* getChunk(std::pair<int, int> address);
    
    void addEntity(BaseEntity *entity);
    
    void render(sf::RenderWindow *window, Camera camera);
};
