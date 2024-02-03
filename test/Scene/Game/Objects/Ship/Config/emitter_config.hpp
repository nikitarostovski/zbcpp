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
    
    EmitterConfig();
    EmitterConfig(EmitterCategory category, float maxSpeed, float acceleration);
    
    bool operator==(const EmitterConfig& rhs) const;
};

std::string emitterTypeString(EmitterCategory emitter);
