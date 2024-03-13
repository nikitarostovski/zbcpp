#pragma once
#include "ship_weapon.hpp"

class PhysicsWorld;

class RocketWeapon : public ShipWeapon
{
protected:
    PhysicsWorld *world;
    sf::Clock lastShotClock;
    
    void spawnBullet();
    void createFixture(b2Body *body) override;
    void renderFixture(sf::RenderWindow *window, Camera camera) override;
public:
    RocketWeapon(WeaponConfig config, PhysicsWorld *world);
    
    void step(float dt) override;
};
