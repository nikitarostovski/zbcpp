#pragma once
#include "Box2D.h"
#include "physics_world.hpp"
#include "fuel_station_building.hpp"
#include "fuel_station_config.hpp"

class FuelStation
{
private:
    PhysicsWorld *world;
public:
    FuelStationBuilding *building;
    FuelStationConfig config;
    
    FuelStation(b2Vec2 pos, FuelStationConfig config, PhysicsWorld *world);
    ~FuelStation();
};
