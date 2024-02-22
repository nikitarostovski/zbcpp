#include "fuel_station.hpp"

FuelStation::FuelStation(b2Vec2 pos, FuelStationConfig config, PhysicsWorld *world)
    : world(world)
    , config(config)
{
    building = new FuelStationBuilding(pos);
    world->addEntity(building);
}

FuelStation::~FuelStation()
{
//    building->destroy();
    delete building;
}
