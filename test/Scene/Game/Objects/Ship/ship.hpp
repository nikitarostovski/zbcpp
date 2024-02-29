#pragma once
#include "physics_world.hpp"
#include "body_entity.hpp"
#include "ship_frame.hpp"
#include "ship_emitter.hpp"
#include "ship_collector.hpp"
#include "ship_config.hpp"

class Ship : public BodyEntity
{
private:
    PhysicsWorld *world;
    float desiredAngle;
    
    void updateConfig(ShipConfig newConfig, b2Vec2 pos);
    bool updateFrameIfNeeded(FrameConfig newFrame, b2Vec2 pos);
    bool updateEmitterIfNeeded(EmitterConfig newEmitter, b2Vec2 pos);
    bool updateCollectorIfNeeded(CollectorConfig newCollector, b2Vec2 pos);
    
    void makeExhaustTrace(b2Vec2 impulse);
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
    void receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius) override;
public:
    ShipFrame *frame;
    ShipEmitter *emitter;
    ShipCollector *collector;
    
    ShipConfig shipConfig;
    std::function<void(float)> onDamageReceive;
    std::function<void(int)> onMaterialCollect;
    
    Ship(b2Vec2 pos, ShipConfig config, PhysicsWorld *world);
    ~Ship();
    
    void step(float dt);
    
    void move(b2Vec2 vector);
    void rotate(float desiredAngle);
    
    void onConfigDidUpdate();
    
    void contactBegin(BodyEntity *entity, b2Fixture *fixture) override;
    void render(sf::RenderWindow *window, Camera camera) override;
};
