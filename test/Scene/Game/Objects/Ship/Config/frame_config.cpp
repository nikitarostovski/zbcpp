#include "frame_config.hpp"

std::string frameTypeString(FrameCategory frame)
{
    switch (frame) {
        case FrameNone:
            return "None";
        case FrameBasic:
            return "Basic";
        case FrameAdvanced:
            return "Advanced";
        default:
            return "-";
    }
}

FrameConfig::FrameConfig()
    : FrameConfig(FrameNone, 0, 1)
{ }

FrameConfig::FrameConfig(FrameCategory category, float armor, float mass)
    : category(category)
    , armor(armor)
    , mass(mass)
{ }

bool FrameConfig::operator==(const FrameConfig& rhs) const
{
    return category == rhs.category && armor == rhs.armor && mass == rhs.mass;
}
