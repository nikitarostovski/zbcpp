#include "ship_config.hpp"

ShipConfig::ShipConfig()
    : ShipConfig(FrameConfig(), EmitterConfig(), CollectorConfig())
{ }

ShipConfig::ShipConfig(FrameConfig frame, EmitterConfig emitter, CollectorConfig collector)
    : frame(frame)
    , emitter(emitter)
    , collector(collector)
{ }
