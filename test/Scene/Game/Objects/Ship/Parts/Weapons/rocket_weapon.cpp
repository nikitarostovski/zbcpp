#include "rocket_weapon.hpp"
#include "collision_category.hpp"
#include "rocket_missle.hpp"
#include "constants.h"
#include "physics_world.hpp"

RocketWeapon::RocketWeapon(WeaponConfig config, PhysicsWorld *world)
    : ShipWeapon(config)
    , world(world)
{
}

void RocketWeapon::createFixture(b2Body *body)
{
    std::vector<b2Vec2> points;
    points.emplace_back(-1.3, 5);
    points.emplace_back(1.3, 5);
    points.emplace_back(1.7, 6);
    points.emplace_back(-1.7, 6);
    
    b2PolygonShape shape;
    shape.Set(points.data(), (int)points.size());

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CategoryShip;
    fixtureDef.filter.maskBits = 0;
    fixtureDef.density = 0.5;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;

    fixture = body->CreateFixture(&fixtureDef);
}

void RocketWeapon::step(float dt)
{
    if (isShooting) {
        auto secondsPassed = lastShotClock.getElapsedTime().asSeconds();
        if (secondsPassed >= 1.0f/config.rate) {
            spawnBullet();
            lastShotClock.restart();
        }
    }
}

void RocketWeapon::spawnBullet()
{
    if (!fixture)
        return;
    
    float f = 30.0f;
    auto rot = b2Rot(fixture->GetBody()->GetAngle() - M_PI_2);
    b2Vec2 imp = b2Vec2(f * rot.c, f * rot.s);
    
    auto pos = fixture->GetBody()->GetPosition();
    auto rocket = new RocketMissle(pos, imp, config.damage);
    world->addEntity(rocket);
}

void RocketWeapon::renderFixture(sf::RenderWindow *window, Camera camera)
{
    if (!fixture)
        return;
    
    b2Body *body = fixture->GetBody();
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(sf::Color(127, 0, 127));
    polygon.setOrigin(localCenter);
    polygon.setPosition((worldCenter.x - camera.x) * camera.scale + window->getSize().x / 2,
                        (worldCenter.y - camera.y) * camera.scale + window->getSize().y / 2);
    polygon.setPointCount(shape->GetVertexCount());
    for (int i = 0; i < shape->GetVertexCount(); i++) {
        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * camera.scale, shape->GetVertex(i).y * camera.scale));
    }
    polygon.setRotation(angle * DEG_PER_RAD);
    window->draw(polygon);
}
