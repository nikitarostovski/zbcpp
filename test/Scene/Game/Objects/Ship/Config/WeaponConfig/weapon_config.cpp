#include "weapon_config.hpp"

WeaponConfig::WeaponConfig()
    : WeaponConfig(0, 0)
{ }

WeaponConfig::WeaponConfig(float damage, float rate)
    : damage(damage)
    , rate(rate)
{ }

bool WeaponConfig::operator==(const WeaponConfig& rhs) const
{
    return damage == rhs.damage && rate == rhs.rate;
}
