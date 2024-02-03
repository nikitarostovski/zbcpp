#pragma once
#include <string>

enum FrameCategory
{
    FrameNone,
    FrameBasic,
    FrameAdvanced
};

struct FrameConfig
{
    FrameCategory category;
    float armor;
    float mass;
    
    FrameConfig();
    FrameConfig(FrameCategory category, float armor, float mass);
    
    bool operator==(const FrameConfig& rhs) const;
};

std::string frameTypeString(FrameCategory frame);
