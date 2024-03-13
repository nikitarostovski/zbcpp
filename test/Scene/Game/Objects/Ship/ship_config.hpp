#pragma once
#include "frame_config.hpp"
#include "emitter_config.hpp"
#include "collector_config.hpp"
#include "weapon_config.hpp"

struct ShipConfig
{
    FrameConfig frame;
    EmitterConfig emitter;
    CollectorConfig collector;
    WeaponConfig weapon;
    
    // TODO: fuel tank
    float fuel;
    float maxFuel;
    
    ShipConfig();
    ShipConfig(FrameConfig frame, EmitterConfig emitter, CollectorConfig collector, WeaponConfig weapon, float fuel, float maxFuel);
};
