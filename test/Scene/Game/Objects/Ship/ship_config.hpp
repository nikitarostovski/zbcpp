#pragma once
#include "frame_config.hpp"
#include "emitter_config.hpp"
#include "collector_config.hpp"

struct ShipConfig
{
    FrameConfig frame;
    EmitterConfig emitter;
    CollectorConfig collector;
    // TODO: fuel tank
    float fuel;
    float maxFuel;
    
    ShipConfig();
    ShipConfig(FrameConfig frame, EmitterConfig emitter, CollectorConfig collector, float fuel, float maxFuel);
};
