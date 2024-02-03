#include "controlled_ship.hpp"
#include "constants.h"
#include "ship_frame_basic.hpp"
#include "ship_frame_advanced.hpp"

ControlledShip::ControlledShip(b2Vec2 pos, ShipConfig config, PhysicsWorld *world)
    : world(world)
    , shipConfig(ShipConfig())
{
    updateFrameIfNeeded(config.frame, pos);
    updateEmitterIfNeeded(config.emitter, pos);
    updateCollectorIfNeeded(config.collector, pos);
    
    shipConfig = config;
}

ControlledShip::~ControlledShip()
{
    delete frame;
    delete emitter;
    delete collector;
}

b2Vec2 ControlledShip::getPosition()
{
    if (!frame)
        return b2Vec2_zero;
    return frame->getPosition();
}

void ControlledShip::onConfigDidUpdate()
{
    b2Vec2 pos = getPosition() * PPM;
    updateFrameIfNeeded(shipConfig.frame, pos);
    updateEmitterIfNeeded(shipConfig.emitter, pos);
    updateCollectorIfNeeded(shipConfig.collector, pos);
}

bool ControlledShip::updateFrameIfNeeded(FrameConfig newFrame, b2Vec2 pos)
{
    if (frame && frame->config == shipConfig.frame)
        return false;
    
    bool hasDamageCallback = false;
    std::function<void(float)> damageCallback;
    if (frame) {
        frame->isDestroying = true;
        if (frame->onDamageReceive) {
            damageCallback = frame->onDamageReceive;
            hasDamageCallback = true;
        }
    }
    
    switch (newFrame.category) {
        case FrameNone:
            frame = nullptr;
            break;
        case FrameBasic:
            frame = new ShipFrameBasic(pos, newFrame);
            break;
        case FrameAdvanced:
            frame = new ShipFrameAdvanced(pos, newFrame);
            break;
    }
    
    world->addEntity(frame);
    
    if (emitter)
        world->addLink(frame, emitter);
    
    if (collector)
        world->addLink(frame, collector);
    
    if (hasDamageCallback && frame)
        frame->onDamageReceive = damageCallback;
    
    return true;
}

bool ControlledShip::updateEmitterIfNeeded(EmitterConfig newEmitter, b2Vec2 pos)
{
    if (emitter && emitter->config == shipConfig.emitter)
        return false;
    
    if (emitter)
        emitter->isDestroying = true;
    
    switch (newEmitter.category) {
        case EmitterNone:
            emitter = nullptr;
            break;
        case EmitterBasic:
            emitter = new ShipEmitter(pos, newEmitter);
            break;
        case EmitterAdvanced:
            emitter = new ShipEmitter(pos, newEmitter);
            break;
    }
    world->addEntity(emitter);
    
    if (frame)
        world->addLink(frame, emitter);
    return true;
}

bool ControlledShip::updateCollectorIfNeeded(CollectorConfig newCollector, b2Vec2 pos)
{
    if (collector && collector->config == shipConfig.collector)
        return false;
    
    if (collector)
        collector->isDestroying = true;
    
    switch (newCollector.category) {
        case CollectorNone:
            collector = nullptr;
            break;
        case CollectorBasic:
            collector = new ShipCollector(pos, newCollector);
            break;
        case CollectorAdvanced:
            collector = new ShipCollector(pos, newCollector);
            break;
    }
    world->addEntity(collector);
    
    if (frame)
        world->addLink(frame, collector);
    return true;
}

void ControlledShip::step(float dt)
{
    if (!frame->body)
        return;
    
    const float rotationSpeed = 15.0f;
//    frame->body->SetTransform(frame->body->GetPosition(), desiredAngle + M_PI_2);
    
    float current = frame->body->GetAngle() + frame->body->GetAngularVelocity() * dt;
    float target = desiredAngle;
    
    float delta = fmod(target, M_PI * 2) - fmod(current, M_PI * 2);
    
    if (delta > M_PI_2)
        delta -= M_PI * 2;
    
    if (delta <= -M_PI_2)
        delta += M_PI * 2;

    frame->body->SetAngularVelocity(delta * rotationSpeed);
}

void ControlledShip::move(b2Vec2 vector)
{
    if (!frame->body)
        return;
    
    float maxVelocity = emitter->config.maxSpeed;
    float acceleration = emitter->config.acceleration;
    
    b2Vec2 imp;
    imp.x = vector.x * acceleration;
    imp.y = vector.y * acceleration;
    
    //    createLiquid(emitter->GetWorldCenter().x, emitter->GetWorldCenter().y, imp * -1, sf::Color::Blue);
    
    frame->body->ApplyLinearImpulse(imp, frame->body->GetWorldCenter(), true);
    
    // Limit max velocity
    b2Vec2 vel = frame->body->GetLinearVelocity();
    if (vel.Length() > maxVelocity) {
        vel = vel / vel.Length() * maxVelocity;
        frame->body->SetLinearVelocity(vel);
    }
}

void ControlledShip::rotate(float desiredAngle)
{
    this->desiredAngle = desiredAngle + M_PI_2;
}

void ControlledShip::render(sf::RenderWindow *window, Camera camera) { }
