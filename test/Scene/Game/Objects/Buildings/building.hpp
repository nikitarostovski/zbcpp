#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "polygon.hpp"
#include "base_entity.hpp"

using namespace PolygonUtils;

class Building : public BaseEntity {
private:
    b2Fixture *contactFixture;
protected:
    b2Vec2 pos;
    float contactRadius;
    sf::Color contactAreaColor;
    
public:
    std::function<void()> onPlayerEnter;
    std::function<void()> onPlayerLeave;
    
    Building(b2Vec2 pos, float contactRadius);
    
    void initializeBody(b2World *world) override;
    void contactBegin(BaseEntity *entity, b2Fixture *fixture) override;
    void contactEnd(BaseEntity *entity, b2Fixture *fixture) override;
    void render(sf::RenderWindow *window, Camera camera) override;
    
    virtual b2Body* initializeBuildingBody(b2World *world) = 0;
    virtual void renderBuilding(sf::RenderWindow *window, Camera camera) = 0;
};
