#pragma once
#include "SFML/Graphics.hpp"
#include "polygon.hpp"
#include "body_entity.hpp"

using namespace PolygonUtils;

class SolidBlock : public BodyEntity
{
private:
    Polygon polygon;
    void createFixture(b2Body *body, Polygon polygon);
    
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    bool canBeCollected;
    
    SolidBlock(Polygon polygon);
    
//    std::vector<SolidBlockDef> split();
    
    void receiveCollision(BodyEntity *entity, float impulse) override;
    void render(sf::RenderWindow *window, Camera camera) override;
    
    std::string getName() override;
};
