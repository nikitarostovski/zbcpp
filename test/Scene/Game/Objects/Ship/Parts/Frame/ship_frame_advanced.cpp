#include "ship_frame_advanced.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipFrameAdvanced::ShipFrameAdvanced(b2Vec2 pos, FrameConfig config)
    : ShipFrame(pos, config, sf::Color(255, 200, 10))
{ }

void ShipFrameAdvanced::initializeBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x / PPM, pos.y / PPM);
    bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> points;
    points.emplace_back(-20 / PPM, 0);
    points.emplace_back(-30 / PPM, 30 / PPM);
    points.emplace_back(-15 / PPM, 50 / PPM);
    points.emplace_back(15 / PPM, 50 / PPM);
    points.emplace_back(30 / PPM, 30 / PPM);
    points.emplace_back(20 / PPM, 0);
    shape.Set(points.data(), (int)points.size());

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CollisionCategory::PlayerFrame;
    fixtureDef.filter.maskBits = CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
    fixtureDef.density = 0.5;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

void ShipFrameAdvanced::receiveCollision(BaseEntity *entity, float impulse)
{
    const std::vector<std::pair<float, float>> impactToDamage = {{5, 1}, {20, 2}, {50, 3}, {100, 10}};
    
    float damage = 0;
    float impact = std::abs(impulse);// * speed;
    
    for (int i = 0; i < impactToDamage.size(); i++) {
        float imp = impactToDamage[i].first;
        if (impact < imp) {
            if (i > 0) {
                damage = impactToDamage[i - 1].second;
                break;
            } else {
                break;
            }
        }
        if (i == impactToDamage.size() - 1) {
            damage = impactToDamage[i].second;
        }
    }
    if (damage == 0 || !onDamageReceive) {
        return;
    }
    damage *= (1.0 - config.armor);
    onDamageReceive((int)damage);
//    printf("IMAPCT: %2.2f, DAMAGE: %1.0f, HEALTH: %3.0f\n", std::abs(impulse), damage, health);
}
