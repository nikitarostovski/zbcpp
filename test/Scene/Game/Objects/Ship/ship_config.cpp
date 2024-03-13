#include "ship_config.hpp"

ShipConfig::ShipConfig()
    : ShipConfig(FrameConfig(), EmitterConfig(), CollectorConfig(), WeaponConfig(), 0, 0)
{ }

ShipConfig::ShipConfig(FrameConfig frame, EmitterConfig emitter, CollectorConfig collector, WeaponConfig weapon, float fuel, float maxFuel)
    : frame(frame)
    , emitter(emitter)
    , collector(collector)
    , weapon(weapon)
    , fuel(fuel)
    , maxFuel(maxFuel)
{ }
