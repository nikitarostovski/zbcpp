#pragma once
#include "SFML/Graphics.hpp"
#include "polygon.hpp"
#include "base_entity.hpp"

using namespace PolygonUtils;

class SolidBlock : public BaseEntity
{
private:
    Polygon polygon;
    CollisionCategory entityType;
    
    void createFixture(Polygon polygon);
    
public:
    bool canBeCollected;
    
    SolidBlock(Polygon polygon);
    
//    std::vector<SolidBlockDef> split();
    
    void initializeBody(b2World *world) override;
    void receiveCollision(BaseEntity *entity, float impulse) override;
    CollisionCategory getEntityType() override;
    void render(sf::RenderWindow *window, Camera camera) override;
};
