#pragma once
#include <string>

struct WeaponConfig
{
    std::string name;
    float damage;
    float rate; // bullets per second
    
    WeaponConfig();
    WeaponConfig(float damage, float rate);
    
    bool operator==(const WeaponConfig& rhs) const;
};
