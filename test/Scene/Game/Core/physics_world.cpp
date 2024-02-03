#include "physics_world.hpp"
#include "constants.h"
#include "solid_block.hpp"

PhysicsWorld::PhysicsWorld()
{
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(this);
    
    b2ParticleSystemDef def;
    def.radius = 2. / PPM;
    b2ParticleSystem *system = world->CreateParticleSystem(&def);
    particle_system = system;
    
    entities = std::vector<BaseEntity *>();
    entitiesToAdd = std::vector<BaseEntity *>();
}

PhysicsWorld::~PhysicsWorld()
{
    world->DestroyParticleSystem(particle_system);
    delete world;
}

// MARK: - Lifecycle

void PhysicsWorld::addGravityField(GravityField *field)
{
    gravityFields.push_back(field);
}

void PhysicsWorld::addEntity(BaseEntity *entity)
{
    entitiesToAdd.push_back(entity);
}

void PhysicsWorld::addLink(BaseEntity *entityA, BaseEntity *entityB)
{
    std::pair<BaseEntity *, BaseEntity *> link{entityA, entityB};
    linksToAdd.push_back(link);
}

void PhysicsWorld::step(float dt)
{
    processDestroyedEntities();
    processNewEntities();
    processNewLinks();
    
    world->Step(dt, 8, 3, 1);
    
    processGravity();
}

// MARK: - Physics

void PhysicsWorld::processDestroyedEntities()
{
//    std::vector<SolidBlockDef> bodiesToAdd;
    
    for (int i = 0; i < entities.size();) {
        BaseEntity *entity = entities[i];
        if (entity->isDestroying) {
            entities.erase(entities.begin() + i);
            entity->destroyBody(world);
            delete entity;
        } else {
            i++;
        }
    }
//        if (entity->isDestroying()) {
//            auto parts = block->split();
//            for (auto def: parts)
//                bodiesToAdd.push_back(def);
//            
//            world->DestroyBody(block->body);
//            blocks.erase(blocks.begin() + i);
//            
//            delete block;
//        }
//    }
//    for (auto def: bodiesToAdd) {
//        instantinateBlock(def);
//    }
}

void PhysicsWorld::processNewEntities()
{
    for (auto entity : entitiesToAdd) {
        entity->initializeBody(world);
        entities.push_back(entity);
    }
    entitiesToAdd.clear();
}

void PhysicsWorld::processNewLinks()
{
    for (int i = 0; i < linksToAdd.size();) {
        b2Body *a = linksToAdd[i].first->body;
        b2Body *b = linksToAdd[i].second->body;
        if (a && b) {
            b2WeldJointDef cjd;
            cjd.collideConnected = false;
            cjd.bodyA = a;
            cjd.bodyB = b;
            world->CreateJoint(&cjd);
            linksToAdd.erase(linksToAdd.begin() + i);
        } else {
            i++;
        }
    }
}

void PhysicsWorld::processGravity()
{
    for (GravityField *field : gravityFields) {
        field->applyGravityToLiquid(particle_system);
        field->applyGravityToEntities(entities);
    }
}

// MARK: - Contacts

void PhysicsWorld::BeginContact(b2Contact* contact) {
    BaseEntity *entityA = static_cast<BaseEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BaseEntity *entityB = static_cast<BaseEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (!entityA || !entityB)
        return;
    
    if (entityA->isDestroying || entityB->isDestroying)
        return;
    
    if (entityA)
        entityA->contactBegin(entityB, contact->GetFixtureA());
    
    if (entityB)
        entityB->contactBegin(entityA, contact->GetFixtureB());
}

void PhysicsWorld::EndContact(b2Contact* contact) {
    BaseEntity *entityA = static_cast<BaseEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BaseEntity *entityB = static_cast<BaseEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (!entityA || !entityB)
        return;
    
    if (entityA->isDestroying || entityB->isDestroying)
        return;
    
    if (entityA)
        entityA->contactEnd(entityB, contact->GetFixtureA());
    
    if (entityB)
        entityB->contactEnd(entityA, contact->GetFixtureB());
}

void PhysicsWorld::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    b2Vec2 i = b2Vec2(impulse->normalImpulses[0] + impulse->normalImpulses[1], impulse->tangentImpulses[0] + impulse->tangentImpulses[1]);
    
    float imp = i.Length();
    if (imp == 0) {
        return;
    }
    
    BaseEntity *entityA = static_cast<BaseEntity *>(contact->GetFixtureA()->GetBody()->GetUserData());
    BaseEntity *entityB = static_cast<BaseEntity *>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if (entityA)
        entityA->contactSolve(entityB, imp);
    if (entityB)
        entityB->contactSolve(entityA, imp);
}

// MARK: - Render

void PhysicsWorld::render(sf::RenderWindow *window, Camera camera)
{
    for (auto field : gravityFields)
        field->render(window, camera);
    
    for (auto entity : entities)
        entity->render(window, camera);
}
