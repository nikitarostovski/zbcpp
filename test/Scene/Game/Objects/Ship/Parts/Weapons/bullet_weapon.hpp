#pragma once
#include "ship_weapon.hpp"

class PhysicsWorld;

class BulletWeapon : public ShipWeapon
{
protected:
    PhysicsWorld *world;
    sf::Clock lastShotClock;
    
    void spawnBullet();
    void createFixture(b2Body *body) override;
    void renderFixture(sf::RenderWindow *window, Camera camera) override;
public:
    BulletWeapon(WeaponConfig config, PhysicsWorld *world);
    
    void step(float dt) override;
};
