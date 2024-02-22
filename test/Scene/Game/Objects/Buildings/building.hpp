#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "polygon.hpp"
#include "body_entity.hpp"

using namespace PolygonUtils;

class Building : public BodyEntity {
private:
    b2Fixture *contactFixture;
protected:
    b2Vec2 pos;
    float contactRadius;
    sf::Color contactAreaColor;
    
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    std::function<void()> onPlayerEnter;
    std::function<void()> onPlayerLeave;
    
    Building(b2Vec2 pos, float contactRadius);
    
    void contactBegin(BodyEntity *entity, b2Fixture *fixture) override;
    void contactEnd(BodyEntity *entity, b2Fixture *fixture) override;
    void render(sf::RenderWindow *window, Camera camera) override;
    
    virtual b2Body* initializeBuildingBody(b2World *world) = 0;
    virtual void renderBuilding(sf::RenderWindow *window, Camera camera) = 0;
};
