#pragma once
#include <SFML/Graphics.hpp>
#include "polygon.hpp"
#include "base_entity.hpp"

using namespace PolygonUtils;

class MarketBuilding : public BaseEntity {
private:
    b2Fixture *mainFixture;
    b2Fixture *contactFixture;
protected:
    b2Vec2 pos;
    sf::Color buildingColor;
    sf::Color contactAreaColor;
    
public:
    std::function<void()> onPlayerEnter;
    std::function<void()> onPlayerLeave;
    
    MarketBuilding(b2Vec2 pos);
    
    CollisionCategory getEntityType() override;
    void initializeBody(b2World *world) override;
    void render(sf::RenderWindow *window, Camera camera) override;
    void contactBegin(BaseEntity *entity, b2Fixture *fixture) override;
    void contactEnd(BaseEntity *entity, b2Fixture *fixture) override;
};
