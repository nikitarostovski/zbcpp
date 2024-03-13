#include "gravity_field_entity.hpp"
#include "body_entity.hpp"
#include "liquid_entity.hpp"
#include "physics_world.hpp"
#include "constants.h"

GravityFieldEntity::GravityFieldEntity(b2Vec2 pos, float radius, float strength, PhysicsWorld *world)
    : BaseEntity()
    , world(world)
{
    this->center = pos;
    this->radius = radius;
    this->strength = strength;
    
    this->shape = sf::CircleShape();
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
}

b2AABB GravityFieldEntity::getAABB()
{
    b2AABB bounds;
    bounds.lowerBound.x = center.x - radius;
    bounds.lowerBound.y = center.y - radius;
    bounds.upperBound.x = center.x + radius;
    bounds.upperBound.y = center.y + radius;
    return bounds;
}

b2Vec2 GravityFieldEntity::getPosition()
{
    return center;
}

void GravityFieldEntity::activate(b2World *world) { }

void GravityFieldEntity::deactivate(b2World *world) { }

void GravityFieldEntity::step(float dt)
{
    if (!world)
        return;
    
    b2Vec2 fieldCenter = getPosition();
    float radiusSquared = radius * radius;
    
    for (int cx = lastChunkXLo; cx <= lastChunkXHi; cx++) {
        for (int cy = lastChunkYLo; cy <= lastChunkYHi; cy++) {
            Chunk *chunk = world->getChunk({cx, cy});
            if (!chunk)
                continue;
            
            for (auto e : chunk->entities) {
                if (e->gravityAppliedOnCurrentStep)
                    continue;
                
                b2Vec2 center = e->getPosition();
                b2Vec2 shift = center - fieldCenter;
                float distSquared = shift.LengthSquared();
                if (distSquared > radiusSquared)
                    continue;
                
                BodyEntity *bodyEntity = dynamic_cast<BodyEntity *>(e);
                if (bodyEntity)
                    applyGravityToBody(bodyEntity);
                
                LiquidEntity *liquidEntity = dynamic_cast<LiquidEntity *>(e);
                if (liquidEntity)
                    applyGravityToLiquid(liquidEntity);
                
                e->gravityAppliedOnCurrentStep = true;
            }
        }
    }
    for (int cx = lastChunkXLo; cx <= lastChunkXHi; cx++) {
        for (int cy = lastChunkYLo; cy <= lastChunkYHi; cy++) {
            Chunk *chunk = world->getChunk({cx, cy});
            if (!chunk)
                continue;
            
            for (auto e : chunk->entities)
                e->gravityAppliedOnCurrentStep = false;
        }
    }
}

void GravityFieldEntity::applyGravityToBody(BodyEntity *entity)
{
    b2Vec2 fieldCenter = getPosition();
    b2Body *body = entity->body;
    if (!body)
        return;
    
    b2Vec2 center = body->GetPosition();
    b2Vec2 shift = center - fieldCenter;
    float dist = shift.Length();
    if (dist > radius)
        return;
    
    b2Vec2 imp = -shift / shift.Normalize() * strength;
    body->ApplyLinearImpulse(imp, body->GetWorldCenter(), true);
}

void GravityFieldEntity::applyGravityToLiquid(LiquidEntity *entity)
{
    if (!DEBUG_PARTICLES_ENABLED)
        return;
    if (!entity->system)
        return;
    const float particleGravityStrength = 0.01f;
    const float particleStrength = strength * entity->system->GetDensity() * particleGravityStrength;
    b2Vec2 fieldCenter = getPosition();
    
    int index = entity->handle->GetIndex();
    b2Vec2 center = entity->system->GetPositionBuffer()[index];
    b2Vec2 shift = center - fieldCenter;
    float dist = shift.Length();
    
    if (dist > radius)
        return;
    
    b2Vec2 imp = -shift / shift.Normalize() * particleStrength;
    entity->system->ParticleApplyLinearImpulse(index, imp);
}

void GravityFieldEntity::render(sf::RenderWindow *window, Camera camera) 
{
    b2Vec2 center = getPosition();
    
    shape.setOrigin(sf::Vector2<float>(radius * camera.scale, radius * camera.scale));
    shape.setPosition((center.x - camera.x) * camera.scale + window->getSize().x / 2,
                      (center.y - camera.y) * camera.scale + window->getSize().y / 2);
    shape.setRadius(radius * camera.scale);
    window->draw(shape);
}
