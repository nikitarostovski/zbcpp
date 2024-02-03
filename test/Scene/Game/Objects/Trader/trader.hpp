#pragma once
#include "Box2D.h"
#include "physics_world.hpp"
#include "market_building.hpp"
#include "trader_config.hpp"

class Trader
{
private:
    PhysicsWorld *world;
    
public:
    MarketBuilding *building;
    TraderConfig config;
    
    Trader(b2Vec2 pos, TraderConfig config, PhysicsWorld *world);
    ~Trader();
};
