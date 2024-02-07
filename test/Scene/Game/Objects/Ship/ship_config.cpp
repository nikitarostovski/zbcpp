#include "ship_config.hpp"

ShipConfig::ShipConfig()
    : ShipConfig(FrameConfig(), EmitterConfig(), CollectorConfig(), 0, 0)
{ }

ShipConfig::ShipConfig(FrameConfig frame, EmitterConfig emitter, CollectorConfig collector, float fuel, float maxFuel)
    : frame(frame)
    , emitter(emitter)
    , collector(collector)
    , fuel(fuel)
    , maxFuel(maxFuel)
{ }
