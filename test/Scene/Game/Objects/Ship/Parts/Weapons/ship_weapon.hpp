#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"
#include "weapon_config.hpp"
#include "camera.hpp"

class PhysicsWorld;

class ShipWeapon {
protected:
    bool isShooting;
    b2Fixture *fixture;
    
    ShipWeapon(WeaponConfig config);
    
public:
    WeaponConfig config;
    
    b2Fixture* getFixture();
    virtual void createFixture(b2Body *body) { };
    
    virtual void step(float dt) {};
    virtual void renderFixture(sf::RenderWindow *window, Camera camera) {};
    
    void startShoot();
    void endShoot();
    
    static ShipWeapon* makeWeapon(WeaponConfig config, PhysicsWorld *world);
};
