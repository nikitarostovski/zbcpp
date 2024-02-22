#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "body_entity.hpp"
#include "collector_config.hpp"

using namespace PolygonUtils;

class ShipCollector : public BodyEntity {
private:
    b2Vec2 pos;
    
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    CollectorConfig config;
    std::function<void(int)> onMaterialCollect;
    
    ShipCollector(b2Vec2 pos, CollectorConfig config);
    
    void render(sf::RenderWindow *window, Camera camera) override;
    
//    void receiveCollision(BaseEntity *entity, float impulse) override;
    void contactBegin(BodyEntity *entity, b2Fixture *fixture) override;
};
