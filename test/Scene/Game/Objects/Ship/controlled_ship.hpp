#pragma once
#include "physics_world.hpp"
#include "base_entity.hpp"
#include "ship_frame.hpp"
#include "ship_emitter.hpp"
#include "ship_collector.hpp"
#include "ship_config.hpp"

class ControlledShip
{
private:
    PhysicsWorld *world;
    float desiredAngle;
    
    void updateConfig(ShipConfig newConfig, b2Vec2 pos);
    bool updateFrameIfNeeded(FrameConfig newFrame, b2Vec2 pos);
    bool updateEmitterIfNeeded(EmitterConfig newEmitter, b2Vec2 pos);
    bool updateCollectorIfNeeded(CollectorConfig newCollector, b2Vec2 pos);

public:
    ShipFrame *frame;
    ShipEmitter *emitter;
    ShipCollector *collector;
    
    ShipConfig shipConfig;
    
    ControlledShip(b2Vec2 pos, ShipConfig config, PhysicsWorld *world);
    ~ControlledShip();
    
    b2Vec2 getPosition();
    
    void step(float dt);
    virtual void render(sf::RenderWindow *window, Camera camera);
    
    void move(b2Vec2 vector);
    void rotate(float desiredAngle);
    
    void onConfigDidUpdate();
};
