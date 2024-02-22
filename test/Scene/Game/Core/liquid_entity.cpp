#include "liquid_entity.hpp"
#include "constants.h"

LiquidEntity::LiquidEntity(b2Vec2 initialPosition, b2Vec2 velocity, b2ParticleColor color)
    : BaseEntity()
{
    this->particleDef = b2ParticleDef();
    
    particleDef.position = initialPosition;
    particleDef.velocity = velocity;
    particleDef.color = color;
    particleDef.flags = b2_waterParticle | b2_viscousParticle;
    particleDef.lifetime = PARTICLE_LIFETIME;
    
    this->shape = sf::CircleShape();
    this->clock = sf::Clock();
}

LiquidEntity::~LiquidEntity()
{
    handle = nullptr;
    system = nullptr;
}

b2AABB LiquidEntity::getAABB()
{
    auto center = getPosition();
    
    b2AABB aabb;
    aabb.lowerBound.x = center.x - PARTICLE_RADIUS;
    aabb.lowerBound.y = center.y - PARTICLE_RADIUS;
    aabb.upperBound.x = center.x + PARTICLE_RADIUS;
    aabb.upperBound.y = center.y + PARTICLE_RADIUS;

    return aabb;
}

b2Vec2 LiquidEntity::getPosition()
{
    if (!handle || !system)
        return particleDef.position;
    
    int index = handle->GetIndex();
    return system->GetPositionBuffer()[index];
}

b2ParticleColor LiquidEntity::getParticleColor()
{
    if (!system || !handle)
        return b2ParticleColor();
    
    int index = handle->GetIndex();
    if (index < 0) {
        isDead = true;
        return b2ParticleColor();
    }
    
    b2ParticleColor result = system->GetColorBuffer()[index];
    float lifetimeRemained = system->GetParticleLifetime(index);
    result.a = 255 * std::max(0.0f, std::min(1.0f, lifetimeRemained / PARTICLE_LIFETIME));
    return result;
}

void LiquidEntity::activate(b2World *world)
{
    if (handle || system || isDead)
        return;
    
    auto time = clock.getElapsedTime();
    if (time.asSeconds() > PARTICLE_LIFETIME) {
        isDead = true;
        return;
    }
    
    system = world->GetParticleSystemList();
    
    int index = system->CreateParticle(particleDef);
    handle = system->GetParticleHandleFromIndex(index);
}

void LiquidEntity::deactivate(b2World *world)
{
    if (!handle || !system)
        return;
    
    particleDef.position = getPosition();
    
    int index = handle->GetIndex();
    system->DestroyParticle(index);
    
    handle = nullptr;
    system = nullptr;
}

void LiquidEntity::render(sf::RenderWindow *window, Camera camera)
{
    if (!handle)
        return;
    b2Vec2 center = getPosition();
    
    b2ParticleColor color = getParticleColor();
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    
    shape.setOrigin(sf::Vector2<float>(PARTICLE_RADIUS * camera.scale, PARTICLE_RADIUS * camera.scale));
    shape.setPosition((center.x - camera.x) * camera.scale + window->getSize().x / 2,
                      (center.y - camera.y) * camera.scale + window->getSize().y / 2);
    shape.setRadius(PARTICLE_RADIUS * camera.scale);
    window->draw(shape);
}

std::string LiquidEntity::getName()
{
    return "liquid";
}
