#pragma once
#include "SFML/Graphics.hpp"
#include "physics_world.hpp"
#include "polygon.hpp"
#include "body_entity.hpp"

using namespace PolygonUtils;

class SolidBlock : public BodyEntity
{
private:
    std::vector<sf::Vertex> cachedVertices;
    
    Polygon polygon;
    float approximateArea;
    bool isImmortal;
    sf::ConvexShape renderShape;
    
    std::vector<sf::ConvexShape> renderShapes;
    
    void createFixture(b2Body *body, Polygon polygon);
    void spawnSubBlocks(b2Vec2 point, float radius, bool hasPointAndRadius);
    void spawnOrb(b2Vec2 point);
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    PhysicsWorld *world;
    SolidBlock(Polygon polygon, bool isImmortal, PhysicsWorld *world);
    
    void receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius) override;
    void receiveDamage(float impulse) override;
    
    void render(sf::RenderWindow *window, Camera camera) override;
    
    std::string getName() override;
};
