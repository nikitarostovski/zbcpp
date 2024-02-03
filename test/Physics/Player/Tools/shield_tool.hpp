#pragma once
#include "constants.h"
#include "player_tool.hpp"

class ShieldTool : public PlayerTool
{
public:
    b2Body *body;
    
    ShieldTool(b2World *world, b2Vec2 pos)
    {
        b2BodyDef boxBodyDef;
        boxBodyDef.position.Set(pos.x, pos.y);
        boxBodyDef.type = b2_dynamicBody;
        body = world->CreateBody(&boxBodyDef);
        
        b2PolygonShape shape;
        std::vector<b2Vec2> points;
        points.emplace_back(0, -20 / PPM);
        points.emplace_back(-25 / PPM, -5 / PPM);
        points.emplace_back(-25 / PPM, 0);
        points.emplace_back(25 / PPM, 0);
        points.emplace_back(25 / PPM, -5 / PPM);
        shape.Set(points.data(), (int)points.size());

        b2FixtureDef fixtureDef;
        fixtureDef.filter.categoryBits = CollisionCategory::PlayerShield;
        fixtureDef.filter.maskBits = CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
        fixtureDef.density = 0.5;
        fixtureDef.friction = 0.5;
        fixtureDef.restitution = 0.2;
        fixtureDef.shape = &shape;

        body->CreateFixture(&fixtureDef);
    }
    
    PlayerToolType getType() override {
        return PlayerToolType::Shield;
    }
    
    b2Body* getBody() override {
        return body;
    }
};
