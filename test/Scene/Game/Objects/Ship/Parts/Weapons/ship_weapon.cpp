#include "ship_weapon.hpp"
#include "constants.h"
#include "collision_category.hpp"

#include "bullet_weapon.hpp"
#include "rocket_weapon.hpp"

ShipWeapon* ShipWeapon::makeWeapon(WeaponConfig config, PhysicsWorld *world)
{
    return new RocketWeapon(config, world);
//    return new BulletWeapon(config, world);
}

ShipWeapon::ShipWeapon(WeaponConfig config)
    : config(config)
{
}

b2Fixture* ShipWeapon::getFixture()
{
    return fixture;
}

void ShipWeapon::startShoot()
{
    isShooting = true;
}

void ShipWeapon::endShoot()
{
    isShooting = false;
}
