#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "base_entity.hpp"
#include "collector_config.hpp"

using namespace PolygonUtils;

class ShipCollector : public BaseEntity {
private:
    b2Vec2 pos;
    
public:
    CollectorConfig config;
    std::function<void(int)> onMaterialCollect;
    
    ShipCollector(b2Vec2 pos, CollectorConfig config);
    
    void initializeBody(b2World *world) override;
    CollisionCategory getEntityType() override;
    void render(sf::RenderWindow *window, Camera camera) override;
    
//    void receiveCollision(BaseEntity *entity, float impulse) override;
    void contactBegin(BaseEntity *entity, b2Fixture *fixture) override;
};
