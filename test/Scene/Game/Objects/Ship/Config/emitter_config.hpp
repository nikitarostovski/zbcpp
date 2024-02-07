#pragma once
#include <string>

enum EmitterCategory
{
    EmitterNone,
    EmitterBasic,
    EmitterAdvanced
};

struct EmitterConfig
{
    EmitterCategory category;
    float maxSpeed;
    float acceleration;
    float consumption;
    
    EmitterConfig();
    EmitterConfig(EmitterCategory category, float maxSpeed, float acceleration, float consumption);
    
    bool operator==(const EmitterConfig& rhs) const;
};

std::string emitterTypeString(EmitterCategory emitter);
