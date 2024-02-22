#include "ship_frame_basic.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipFrameBasic::ShipFrameBasic(b2Vec2 pos, FrameConfig config)
    : ShipFrame(pos, config, sf::Color::Yellow)
{ }

b2AABB ShipFrameBasic::getInitialAABB()
{
    b2AABB result;
    result.lowerBound.x = pos.x - 2;
    result.lowerBound.y = pos.y + 0;
    result.upperBound.x = pos.x + 3;
    result.upperBound.y = pos.y + 5;
    return result;
}

b2Body* ShipFrameBasic::createBody(b2World *world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> points;
    points.emplace_back(-2, 0);
    points.emplace_back(-3, 3);
    points.emplace_back(-1.5, 5);
    points.emplace_back(1.5, 5);
    points.emplace_back(3, 3);
    points.emplace_back(2, 0);
    shape.Set(points.data(), (int)points.size());

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CollisionCategory::PlayerFrame;
    fixtureDef.filter.maskBits = CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
    fixtureDef.density = config.mass;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
    return body;
}

void ShipFrameBasic::receiveCollision(BodyEntity *entity, float impulse)
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
    float armorValue = std::min(1.0f, std::max(0.0f, config.armor));
    
    damage *= (1.0 - armorValue);
    onDamageReceive(damage);
//    printf("IMAPCT: %2.2f, DAMAGE: %1.0f, HEALTH: %3.0f\n", std::abs(impulse), damage, health);
}
