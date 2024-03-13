#include "ship.hpp"
#include "constants.h"
#include "liquid_entity.hpp"

Ship::Ship(b2Vec2 pos, ShipConfig config, PhysicsWorld *world)
    : BodyEntity(pos, CategoryShip)
    , world(world)
    , shipConfig(ShipConfig())
{
    updateFrameIfNeeded(config.frame, pos);
    updateEmitterIfNeeded(config.emitter, pos);
    updateCollectorIfNeeded(config.collector, pos);
    updateWeaponInNeeded(config.weapon, pos);
    
    shipConfig = config;
}

Ship::~Ship()
{
    delete frame;
    delete emitter;
    delete collector;
}

b2Body* Ship::createBody(b2World *world)
{
    auto pos = getPosition();
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.type = b2_dynamicBody;
    b2Body *body = world->CreateBody(&bodyDef);
    body->SetUserData(this);
    
    this->body = body;
    onConfigDidUpdate();
    
    return body;
}

b2AABB Ship::getInitialAABB()
{
    auto pos = getPosition();
    b2AABB result;
    result.lowerBound.x = pos.x;
    result.lowerBound.y = pos.y;
    result.upperBound.x = pos.x;
    result.upperBound.y = pos.y;
    
    return result;
}

void Ship::onConfigDidUpdate()
{
    b2Vec2 pos = getPosition();
    updateFrameIfNeeded(shipConfig.frame, pos);
    updateEmitterIfNeeded(shipConfig.emitter, pos);
    updateCollectorIfNeeded(shipConfig.collector, pos);
    updateWeaponInNeeded(shipConfig.weapon, pos);
}

bool Ship::updateFrameIfNeeded(FrameConfig newFrame, b2Vec2 pos)
{
    if (!body)
        return false;
    
    if (frame && frame->config == shipConfig.frame && frame->getFixture() != nullptr) {
        return false;
    }
    
    if (frame && frame->getFixture()) {
        body->DestroyFixture(frame->getFixture());
    }
    frame = new ShipFrame(newFrame, body);
    return true;
}

bool Ship::updateEmitterIfNeeded(EmitterConfig newEmitter, b2Vec2 pos)
{
    if (!body)
        return false;
    
    if (emitter && emitter->config == shipConfig.emitter && emitter->getFixture() != nullptr) {
        return false;
    }
    
    if (emitter && emitter->getFixture()) {
        body->DestroyFixture(emitter->getFixture());
    }
    emitter = new ShipEmitter(newEmitter, body);
    return true;
}

bool Ship::updateCollectorIfNeeded(CollectorConfig newCollector, b2Vec2 pos)
{
    if (!body)
        return false;
    
    if (collector && collector->config == shipConfig.collector && collector->getFixture() != nullptr) {
        return false;
    }
    
    if (collector && collector->getFixture()) {
        body->DestroyFixture(collector->getFixture());
    }
    collector = new ShipCollector(newCollector, body);
    collector->onOrbCollected = [this](Orb *orb){
        this->onMaterialCollect(1);
    };
    return true;
}

bool Ship::updateWeaponInNeeded(WeaponConfig newWeapon, b2Vec2 pos)
{
    if (!body)
        return false;
    
    if (weapon && weapon->config == shipConfig.weapon && weapon->getFixture() != nullptr) {
        return false;
    }
    
    if (weapon && weapon->getFixture()) {
        body->DestroyFixture(collector->getFixture());
    }
    weapon = ShipWeapon::makeWeapon(newWeapon, world);
    weapon->createFixture(body);
    return true;
}

void Ship::startFire()
{
    if (!weapon)
        return;
    weapon->startShoot();
}

void Ship::stopFire()
{
    if (!weapon)
        return;
    weapon->endShoot();
}

void Ship::step(float dt)
{
    if (!body)
        return;
    
    const float rotationSpeed = 15.0f;
//    frame->body->SetTransform(frame->body->GetPosition(), desiredAngle + M_PI_2);
    
    float current = body->GetAngle() + body->GetAngularVelocity() * dt;
    float target = desiredAngle;
    
    float delta = fmod(target, M_PI * 2) - fmod(current, M_PI * 2);
    
    if (delta > M_PI_2)
        delta -= M_PI * 2;
    
    if (delta <= -M_PI_2)
        delta += M_PI * 2;

    body->SetAngularVelocity(delta * rotationSpeed);
    
    if (collector) {
        collector->step(dt);
    }
    if (weapon) {
        weapon->step(dt);
    }
}

void Ship::makeExhaustTrace(b2Vec2 impulse)
{
    sf::Color color = sf::Color::Blue;
    b2Vec2 pos = getPosition();
    float dist = 6.0f;
    float angle = body->GetAngle() + M_PI_2;

    pos.x += dist * cos(angle);
    pos.y += dist * sin(angle);
    
    b2ParticleColor pColor{color.r, color.g, color.b, 255};
    
    auto liquid = new LiquidEntity(pos, impulse, pColor);
    world->addEntity(liquid);
}

void Ship::move(b2Vec2 vector)
{
    if (!body || vector == b2Vec2_zero)
        return;
    
    float consumption = emitter->config.consumption;
    float maxVelocity = emitter->config.maxSpeed;
    float acceleration = emitter->config.acceleration;
    
    float fuel = shipConfig.fuel;
    if (fuel == 0)
        return;
    
    fuel = std::max(0.0f, fuel - consumption);
    shipConfig.fuel = fuel;
    
    b2Vec2 imp;
    imp.x = vector.x * acceleration;
    imp.y = vector.y * acceleration;
    
    body->ApplyLinearImpulse(imp, body->GetWorldCenter(), true);
    
    // Limit max velocity
    b2Vec2 vel = body->GetLinearVelocity();
    if (vel.Length() > maxVelocity) {
        vel = vel / vel.Length() * maxVelocity;
        body->SetLinearVelocity(vel);
    }
    
    makeExhaustTrace(imp * -1);
}

void Ship::rotate(float desiredAngle)
{
    this->desiredAngle = desiredAngle + M_PI_2;
}

void Ship::render(sf::RenderWindow *window, Camera camera) {
    if (frame) {
        frame->renderFixture(window, camera);
    }
    if (emitter) {
        emitter->renderFixture(window, camera);
    }
    if (collector) {
        collector->renderFixture(window, camera);
    }
    if (weapon) {
        weapon->renderFixture(window, camera);
    }
}

void Ship::contactBegin(BodyEntity *entity, b2Fixture *fixture)
{
    if (!entity) {
        BodyEntity::contactBegin(entity, fixture);
        return;
    }
    Orb *orb = dynamic_cast<Orb *>(entity);
    if (collector && orb) {
        collector->receiveCollision(orb);
    }
}

void Ship::receiveCollision(BodyEntity *entity, float impulse, b2Vec2 point, float radius)
{
    const std::vector<std::pair<float, float>> impactToDamage = {{100, 1}, {300, 2}, {500, 3}, {800, 10}};
    
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
    float armorValue = std::min(1.0f, std::max(0.0f, frame->config.armor));
    
    damage *= (1.0 - armorValue);
    onDamageReceive(damage);
    printf("!!! IMPACT: %2.2f, DAMAGE: %1.0f\n", std::abs(impulse), damage);
}
