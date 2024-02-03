#include "emitter_config.hpp"

std::string emitterTypeString(EmitterCategory emitter)
{
    switch (emitter) {
        case EmitterNone:
            return "None";
        case EmitterBasic:
            return "Basic";
        case EmitterAdvanced:
            return "Advanced";
    }
}

EmitterConfig::EmitterConfig()
    : EmitterConfig(EmitterNone, 0, 0)
{ }

EmitterConfig::EmitterConfig(EmitterCategory category, float maxSpeed, float acceleration)
    : category(category)
    , maxSpeed(maxSpeed)
    , acceleration(acceleration)
{ }

bool EmitterConfig::operator==(const EmitterConfig& rhs) const
{
    return category == rhs.category && maxSpeed == rhs.maxSpeed && acceleration == rhs.acceleration;
}
