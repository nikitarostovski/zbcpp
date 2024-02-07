#include "gravity_field.hpp"
#include "constants.h"

GravityField::GravityField(float x, float y, float radius, float strength)
{
    this->x = x / PPM;
    this->y = y / PPM;
    this->radius = radius / PPM;
    this->strength = strength;
}

void GravityField::applyGravityToEntities(std::vector<BaseEntity *> entities)
{
    b2Vec2 fieldCenter{x, y};
    
    for (BaseEntity *entity : entities) {
        b2Body *body = entity->body;
        if (!body || body->GetType() != b2_dynamicBody)
            continue;
        
        b2Vec2 center = body->GetPosition();
        b2Vec2 shift = center - fieldCenter;
        float dist = shift.Length();
        if (dist > radius)
            continue;
        
        b2Vec2 imp = -shift / shift.Normalize() * strength;
        body->ApplyLinearImpulse(imp, body->GetWorldCenter(), true);
    }
}

void GravityField::applyGravityToLiquid(b2ParticleSystem *particleSystem)
{
    const float particleGravityStrength = 0.01f;
    const float particleStrength = strength * particleSystem->GetDensity() * particleGravityStrength;
    b2Vec2 fieldCenter{x, y};
    
    int count = particleSystem->GetParticleCount();
    b2Vec2* positions = particleSystem->GetPositionBuffer();
    
    for (int i = 0; i < count; i++) {
        b2Vec2 center = positions[i];
        b2Vec2 shift = center - fieldCenter;
        float dist = shift.Length();
        
        if (dist > radius)
            continue;
        
        b2Vec2 imp = -shift / shift.Normalize() * particleStrength;
        particleSystem->ParticleApplyLinearImpulse(i, imp);
    }
}

void GravityField::render(sf::RenderWindow *window, Camera camera)
{
    b2Vec2 center = b2Vec2(x, y);
    
    sf::CircleShape gravity_field;
    gravity_field.setOrigin(sf::Vector2<float>(radius * PPM, radius * PPM));
    gravity_field.setPosition((center.x - camera.x) * PPM + window->getSize().x / 2,
                              (center.y - camera.y) * PPM + window->getSize().y / 2);
    gravity_field.setRadius(radius * PPM);
    gravity_field.setFillColor(sf::Color::Transparent);
    gravity_field.setOutlineColor(sf::Color::White);
    gravity_field.setOutlineThickness(2);
    window->draw(gravity_field);
}
