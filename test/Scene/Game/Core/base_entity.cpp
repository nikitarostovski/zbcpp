#include "base_entity.hpp"

BaseEntity::BaseEntity(bool canBeDestroyed)
{
    this->canBeDestroyed = canBeDestroyed;
    this->isDestroying = false;
}

BaseEntity::~BaseEntity()
{
    body = nullptr;
}

b2Vec2 BaseEntity::getPosition()
{
    if (!body)
        return b2Vec2_zero;
    
    return body->GetWorldCenter();
}

void BaseEntity::destroyBody(b2World *world)
{
    if (!body)
        return;
    world->DestroyBody(body);
}

void BaseEntity::destroy()
{
    if (!canBeDestroyed)
        return;
    isDestroying = true;
}

void BaseEntity::contactBegin(BaseEntity *entity, b2Fixture *fixture) {
    if (entity)
        collisionMap[entity] = true;
}

void BaseEntity::contactEnd(BaseEntity *entity, b2Fixture *fixture) { }

void BaseEntity::contactSolve(BaseEntity *entity, float impulse) {
    if (!entity)
        return;
    
    if (collisionMap[entity] == false)
        return;
    receiveCollision(entity, impulse);
    collisionMap.erase(entity);
}
