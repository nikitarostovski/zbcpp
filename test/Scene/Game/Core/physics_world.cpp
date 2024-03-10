#include "physics_world.hpp"
#include "constants.h"
#include "solid_block.hpp"
#include "body_entity.hpp"

PhysicsWorld::PhysicsWorld()
{
    lastChunkUpdateCenter = b2Vec2(b2_maxFloat / 2, b2_maxFloat / 2);
    
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(this);
    
    b2ParticleSystemDef def;
    def.radius = PARTICLE_RADIUS;
    def.density = 0.3f;
    def.viscousStrength = 0.8f;
    b2ParticleSystem *system = world->CreateParticleSystem(&def);
    particle_system = system;
    
    for (auto t : allMaterialTypes) {
        terrainRenderers[t] = new TerrainRenderer(t);
    }
}

PhysicsWorld::~PhysicsWorld()
{
    world->DestroyParticleSystem(particle_system);
    delete world;
}

Chunk* PhysicsWorld::getChunk(std::pair<int, int> address)
{
    return chunks[address];
}

void PhysicsWorld::addEntity(BaseEntity *entity)
{
    entity->checkIfChunkChanged();
    for (int x = entity->lastChunkXLo; x <= entity->lastChunkXHi; x++) {
        for (int y = entity->lastChunkYLo; y <= entity->lastChunkYHi; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (!chunk) {
                chunk = new Chunk();
                chunks[address] = chunk;
            }
            chunk->entities.push_back(entity);
        }
    }
    int r1MinX = std::min(activeChunkX.x, activeChunkX.x + activeChunkX.y);
    int r1MaxX = std::max(activeChunkX.x, activeChunkX.x + activeChunkX.y);
    int r1MinY = std::min(activeChunkY.x, activeChunkY.x + activeChunkY.y);
    int r1MaxY = std::max(activeChunkY.x, activeChunkY.x + activeChunkY.y);

    int r2MinX = std::min(entity->lastChunkXLo, entity->lastChunkXHi);
    int r2MaxX = std::max(entity->lastChunkXLo, entity->lastChunkXHi);
    int r2MinY = std::min(entity->lastChunkYLo, entity->lastChunkYHi);
    int r2MaxY = std::max(entity->lastChunkYLo, entity->lastChunkYHi);

    int interLeft   = std::max(r1MinX, r2MinX);
    int interTop    = std::max(r1MinY, r2MinY);
    int interRight  = std::min(r1MaxX, r2MaxX);
    int interBottom = std::min(r1MaxY, r2MaxY);

    bool intersects = (interLeft <= interRight) && (interTop <= interBottom);
    if (intersects) {
        entitiesToActivate.push_back(entity);
    }
}

void PhysicsWorld::updateChunks(b2Vec2 center, float width, float height)
{
    float dist = (center - lastChunkUpdateCenter).Length();
    if (dist < CHUNK_SIZE / 4)
        return;
    
    float chunksWidth = width / CHUNK_SIZE;
    float chunksHeight = height / CHUNK_SIZE;
    float chunkX = center.x / CHUNK_SIZE - 0.5f;
    float chunkY = center.y / CHUNK_SIZE - 0.5f;
    
    int xStart = std::floor(chunkX - chunksWidth / 2);
    int xEnd = std::ceil(chunkX + chunksWidth / 2);
    int yStart = std::floor(chunkY - chunksHeight / 2);
    int yEnd = std::ceil(chunkY + chunksHeight / 2);
    
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            if (x < xStart || x > xEnd || y < yStart || y > yEnd) {
                auto address = std::pair<int, int>(x, y);
                bool alreadyInList = false;
                for (int i = 0; i < chunksToDeactivate.size(); i++) {
                    if (chunksToDeactivate[i] == address) {
                        alreadyInList = true;
                        break;
                    }
                }
                if (!alreadyInList)
                    chunksToDeactivate.push_back(address);
            }
        }
    }
    for (int x = xStart; x <= xEnd; x++) {
        for (int y = yStart; y <= yEnd; y++) {
            if (x >= activeChunkX.x && x <= activeChunkX.x + activeChunkX.y &&
                y >= activeChunkY.x && y <= activeChunkY.x + activeChunkY.y) {
                continue;
            }
            auto address = std::pair<int, int>(x, y);
            bool alreadyInList = false;
            for (int i = 0; i < chunksToActivate.size(); i++) {
                if (chunksToActivate[i] == address) {
                    alreadyInList = true;
                    break;
                }
            }
            if (!alreadyInList)
                chunksToActivate.push_back(address);
        }
    }
    
    activeChunkX.x = xStart;
    activeChunkY.x = yStart;
    activeChunkX.y = xEnd - xStart;
    activeChunkY.y = yEnd - yStart;
    
    lastChunkUpdateCenter = center;
}

// MARK: - Lifecycle

void PhysicsWorld::step(float dt)
{
    processChunks();
    updateEntitesChunks();
    processDestroyedEntities();
    processNewEntities();
    
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (!chunk)
                continue;
            
            for (auto entity : chunk->entities) {
                entity->step(dt);
            }
        }
    }
    
    world->Step(dt, 8, 3, 1);
}

void PhysicsWorld::updateEntitesChunks()
{
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (!chunk)
                continue;
            
            for (int i = 0; i < chunk->entities.size(); i++) {
                auto e = chunk->entities[i];
                int lastXLo = e->lastChunkXLo;
                int lastXHi = e->lastChunkXHi;
                int lastYLo = e->lastChunkYLo;
                int lastYHi = e->lastChunkYHi;
                
                if (e->checkIfChunkChanged()) {
                    int newXLo = e->lastChunkXLo;
                    int newXHi = e->lastChunkXHi;
                    int newYLo = e->lastChunkYLo;
                    int newYHi = e->lastChunkYHi;
                    
                    // delete entity from old chunks
                    for (int xx = lastXLo; xx <= lastXHi; xx++) {
                        for (int yy = lastYLo; yy <= lastYHi; yy++) {
                            if (xx < newXLo || xx > newXHi || yy < newYLo || yy > newYHi) {
                                auto addressToRemove = std::pair<int, int>(xx, yy);
                                Chunk *chunkToRemove = chunks[addressToRemove];
                                if (!chunkToRemove)
                                    continue;
                                for (int ii = 0; ii < chunkToRemove->entities.size(); ii++) {
                                    if (chunkToRemove->entities[ii] == e) {
                                        chunkToRemove->entities.erase(chunkToRemove->entities.begin() + ii);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    
                    // add entity to new chunks
                    for (int xx = newXLo; xx <= newXHi; xx++) {
                        for (int yy = newYLo; yy <= newYHi; yy++) {
                            auto addressToAdd = std::pair<int, int>(xx, yy);
                            Chunk *chunkToAdd = chunks[addressToAdd];
                            if (!chunkToAdd) {
                                chunkToAdd = new Chunk();
                                chunks[addressToAdd] = chunkToAdd;
                            }
                            bool alreadyInChunk = false;
                            for (int ii = 0; ii < chunkToAdd->entities.size(); ii++) {
                                if (chunkToAdd->entities[ii] == e) {
                                    alreadyInChunk = true;
                                    break;
                                }
                            }
                            if (!alreadyInChunk) {
                                chunkToAdd->entities.push_back(e);
                            }
                        }
                    }
                }
            }
        }
    }
}

void PhysicsWorld::processNewEntities() {
    chunkClock.restart();
    for (int i = 0; i < entitiesToActivate.size();) {
        if (chunkClock.getElapsedTime().asMilliseconds() > CHUNK_ACTIVATION_TIME_LIMIT_MS) {
            break;
        }
        auto entity = entitiesToActivate[i];
        entity->activate(world);
        entitiesToActivate.erase(entitiesToActivate.begin() + i);
    }
}

void PhysicsWorld::processDestroyedEntities()
{
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (chunk)
                chunk->removeDeadEntities(world);
        }
    }
}

void PhysicsWorld::processChunks()
{
    chunkClock.restart();
    
    for (int i = 0; i < chunksToActivate.size();) {
        auto address = chunksToActivate[i];
        if (chunkClock.getElapsedTime().asMilliseconds() > CHUNK_ACTIVATION_TIME_LIMIT_MS) {
            break;
        }
        Chunk *chunk = chunks[address];
        if (!chunk) {
            chunk = new Chunk();
            chunks[address] = chunk;
        }
        for (auto entity : chunk->entities) {
            entitiesToActivate.push_back(entity);
        }
        chunksToActivate.erase(chunksToActivate.begin() + i);
    }
    
    for (int i = 0; i < chunksToDeactivate.size();) {
        auto address = chunksToDeactivate[i];
        if (chunkClock.getElapsedTime().asMilliseconds() > CHUNK_ACTIVATION_TIME_LIMIT_MS) {
            break;
        }
        Chunk *chunk = chunks[address];
        if (!chunk) {
            i++;
            continue;
        }
        for (auto entity : chunk->entities) {
            int r1MinX = std::min(activeChunkX.x, activeChunkX.x + activeChunkX.y);
            int r1MaxX = std::max(activeChunkX.x, activeChunkX.x + activeChunkX.y);
            int r1MinY = std::min(activeChunkY.x, activeChunkY.x + activeChunkY.y);
            int r1MaxY = std::max(activeChunkY.x, activeChunkY.x + activeChunkY.y);

            int r2MinX = std::min(entity->lastChunkXLo, entity->lastChunkXHi);
            int r2MaxX = std::max(entity->lastChunkXLo, entity->lastChunkXHi);
            int r2MinY = std::min(entity->lastChunkYLo, entity->lastChunkYHi);
            int r2MaxY = std::max(entity->lastChunkYLo, entity->lastChunkYHi);

            int interLeft   = std::max(r1MinX, r2MinX);
            int interTop    = std::max(r1MinY, r2MinY);
            int interRight  = std::min(r1MaxX, r2MaxX);
            int interBottom = std::min(r1MaxY, r2MaxY);

            bool intersects = (interLeft <= interRight) && (interTop <= interBottom);
            
            if (!intersects)
                entity->deactivate(world);
        }
        chunksToDeactivate.erase(chunksToDeactivate.begin() + i);
    }
}

// MARK: - Contacts

void PhysicsWorld::BeginContact(b2Contact* contact) {
    BodyEntity *entityA = static_cast<BodyEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BodyEntity *entityB = static_cast<BodyEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (!entityA || !entityB)
        return;
    
    if (entityA->isDead || entityB->isDead)
        return;
    
    entityA->contactBegin(entityB, contact->GetFixtureA());
    entityB->contactBegin(entityA, contact->GetFixtureB());
}

void PhysicsWorld::EndContact(b2Contact* contact) {
    BodyEntity *entityA = static_cast<BodyEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BodyEntity *entityB = static_cast<BodyEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (!entityA || !entityB)
        return;
    
    if (entityA->isDead || entityB->isDead)
        return;
    
    
    entityA->contactEnd(entityB, contact->GetFixtureA());
    entityB->contactEnd(entityA, contact->GetFixtureB());
    
    disabledContacts.erase({entityA, entityB});
}

void PhysicsWorld::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    float mass = (contact->GetFixtureA()->GetBody()->GetMass() + contact->GetFixtureB()->GetBody()->GetMass()) / 2;
    // impulse adjusted for neadby bodies (shockwave damage)
    float impulseAdjusted = impulse->normalImpulses[0] / mass;

    // imulses adjusted for colliding bodies personally
    float ia = impulse->normalImpulses[0] / contact->GetFixtureB()->GetBody()->GetMass();
    float ib = impulse->normalImpulses[0] / contact->GetFixtureA()->GetBody()->GetMass();
    
    if (impulseAdjusted < COLLLISION_THRESHOLD) {
        return;
    }
    
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);
    b2Vec2 point = manifold.points[0];
    
    BodyEntity *entityA = static_cast<BodyEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BodyEntity *entityB = static_cast<BodyEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (disabledContacts[{entityA, entityB}]) {
        return;
    }
    
    float damageRadius = COLLLISION_RADIUS;
    
    // process contact for bodies a and b
    entityA->receiveCollision(entityB, ia/*impulseAdjusted*/, point, damageRadius);
    entityB->receiveCollision(entityA, ib, point, damageRadius);
    
    // process all other bodies separately
    int xLo = std::floor((point.x - damageRadius) / CHUNK_SIZE);
    int yLo = std::floor((point.y - damageRadius) / CHUNK_SIZE);
    int xHi = std::ceil((point.x + damageRadius) / CHUNK_SIZE);
    int yHi = std::ceil((point.y + damageRadius) / CHUNK_SIZE);
    
    std::map<BodyEntity *, bool> processedEntities;
    
    for (int x = xLo; x <= xHi; x++) {
        for (int y = yLo; y <= yHi; y++) {
            std::pair<int, int> address = {x, y};
            Chunk *chunk = chunks[address];
            if (!chunk)
                continue;
            
            for (int i = 0; i < chunk->entities.size(); i++) {
                auto e = chunk->entities[i];
                if (e->isDead || e == entityA || e == entityB)
                    continue;
                BodyEntity *be = dynamic_cast<BodyEntity *>(e);
                if (be && !processedEntities[be]) {
                    float dist = (be->getPosition() - point).Length();
                    float impulsePersonal = impulseAdjusted * (1.0f - dist / damageRadius);
                    be->receiveDamage(impulsePersonal);
                    processedEntities[be] = true;
                }
            }
        }
    }
    disabledContacts[{entityA, entityB}] = true;
}

// MARK: - Render

void PhysicsWorld::render(sf::RenderWindow *window, Camera camera)
{
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (chunk)
                chunk->render(window, camera, address);
        }
    }
    
    for (int x = activeChunkX.x; x <= activeChunkX.x + activeChunkX.y; x++) {
        for (int y = activeChunkY.x; y <= activeChunkY.x + activeChunkY.y; y++) {
            auto address = std::pair<int, int>(x, y);
            Chunk *chunk = chunks[address];
            if (!chunk) {
                continue;
            }
            for (auto entity : chunk->entities)
                entity->render(window, camera);
        }
    }
    
    
    for (auto pair : terrainRenderers) {
        pair.second->setPosition(-camera.x * camera.scale + window->getSize().x / 2,
                                 -camera.y * camera.scale + window->getSize().y / 2);
        pair.second->setScale(camera.scale, camera.scale);
        window->draw(*pair.second);
        pair.second->reset();
    }
}
