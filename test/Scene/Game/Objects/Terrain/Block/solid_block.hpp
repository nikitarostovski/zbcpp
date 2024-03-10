#pragma once
#include "SFML/Graphics.hpp"
#include "physics_world.hpp"
#include "polygon.hpp"
#include "body_entity.hpp"

using namespace PolygonUtils;

class SolidBlock : public BodyEntity
{
private:
    b2Vec2 parentPolygonCenterOffset;
    
    std::vector<b2FixtureDef *> fixturesToDestroy;
    
    Material material;
    
    b2AABB initialAABB;
    b2BodyDef bodyDef;
    
    std::vector<b2FixtureDef *> fixtureDefs;
    std::map<b2FixtureDef *, Polygon> fixtureDefPolygons;
    
    float approximateArea;
    bool isImmortal;
    
    void createFixture(b2Body *body, b2FixtureDef *def);
    void spawnSubBlocks(b2Vec2 point, float radius, bool hasPointAndRadius);
    void spawnOrb(b2Vec2 point);
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    PhysicsWorld *world;
    
    SolidBlock(Polygon polygon, bool isImmortal, PhysicsWorld *world);
    
    void step(float dt) override;
    
    void receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius) override;
    void receiveDamage(float impulse) override;
    
    void render(sf::RenderWindow *window, Camera camera) override;
    
    std::string getName() override;
};
