#include "body_entity.hpp"
#include "constants.h"

BodyEntity::BodyEntity(b2Vec2 initialPosition, CollisionCategory collisionCategory)
    : BaseEntity()
{
    this->didSetupAABB = false;
    this->lastKnownPosition = initialPosition;
    this->lastKnownAngle = 0;
    this->body = nullptr;
    this->collisionCategory = collisionCategory;
    this->didSetupAABB = false;
}

BodyEntity::~BodyEntity()
{
    body = nullptr;
}

b2AABB BodyEntity::getAABB()
{
    if (!didSetupAABB) {
        lastKnownAABB = getInitialAABB();
        didSetupAABB = true;
    }
    if (!body)
        return lastKnownAABB;
    
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    b2Fixture* fixture = body->GetFixtureList();
    while (fixture != nullptr) {
         const b2Shape *shape = fixture->GetShape();
         const int childCount = shape->GetChildCount();
         for (int child = 0; child < childCount; ++child) {
                b2AABB shapeAABB;
                shape->ComputeAABB(&shapeAABB, body->GetTransform(), child);
                aabb.Combine(shapeAABB);
        }
        fixture = fixture->GetNext();
    }
    return aabb;
}

b2Vec2 BodyEntity::getPosition()
{
    if (!body)
        return lastKnownPosition;
    
    return body->GetPosition();
}

void BodyEntity::activate(b2World *world)
{
    if (body || isDead)
        return;
    body = createBody(world);
    body->SetTransform(lastKnownPosition, lastKnownAngle);
}

void BodyEntity::deactivate(b2World *world)
{
    if (!body)
        return;
    lastKnownAABB = getAABB();
    lastKnownPosition = body->GetPosition();
    lastKnownAngle = body->GetAngle();
    world->DestroyBody(body);
    body = nullptr;
}
