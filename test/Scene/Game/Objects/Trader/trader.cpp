#include "trader.hpp"

Trader::Trader(b2Vec2 pos, TraderConfig config, PhysicsWorld *world)
    : world(world)
    , config(config)
{
    building = new MarketBuilding(pos);
    world->addEntity(building);
}

Trader::~Trader()
{
//    building->destroy();
    delete building;
}
