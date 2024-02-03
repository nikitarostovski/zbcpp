//#include "player.hpp"
//#include "constants.h"
//#include "collector_tool.hpp"
//#include "shield_tool.hpp"
//
//int Player::getEntityType() { return 1; }
//
//b2Vec2 Player::getSpeed() { return frame->GetLinearVelocity(); }
//
//Player::Player(PhysicsWorld *world_, b2Vec2 pos)
//    : world(world_)
//    , maxVelocity(25)
//{
//    health = 
//    maxHealth = 25;
//    
//    collectedMaterialCount = 0;
//    
//    createFrame(pos);
//    createEmitter();
//    
//    selectTool(PlayerToolType::Collector);
//}
//
//void Player::createFrame(b2Vec2 pos)
//{
//    b2BodyDef boxBodyDef;
//    boxBodyDef.position.Set(pos.x, pos.y);
//    boxBodyDef.type = b2_dynamicBody;
//    frame = world->world->CreateBody(&boxBodyDef);
//    frame->SetUserData(this);
//    
//    b2PolygonShape shape;
//    std::vector<b2Vec2> points;
//    points.emplace_back(-20 / PPM, 0);
//    points.emplace_back(-30 / PPM, 30 / PPM);
//    points.emplace_back(-15 / PPM, 50 / PPM);
//    points.emplace_back(15 / PPM, 50 / PPM);
//    points.emplace_back(30 / PPM, 30 / PPM);
//    points.emplace_back(20 / PPM, 0);
//    shape.Set(points.data(), (int)points.size());
//
//    b2FixtureDef fixtureDef;
//    fixtureDef.filter.categoryBits = CollisionCategory::PlayerFrame;
//    fixtureDef.filter.maskBits = CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
//    fixtureDef.density = 0.5;
//    fixtureDef.friction = 0.5;
//    fixtureDef.restitution = 0.2;
//    fixtureDef.shape = &shape;
//
//    frame->CreateFixture(&fixtureDef);
//}
//
//void Player::createEmitter()
//{
//    b2BodyDef boxBodyDef;
//    boxBodyDef.position.Set(frame->GetPosition().x, frame->GetPosition().y);
//    boxBodyDef.type = b2_dynamicBody;
//    emitter = world->world->CreateBody(&boxBodyDef);
//    
//    b2PolygonShape shape;
//    std::vector<b2Vec2> points;
//    points.emplace_back(-15 / PPM, 50 / PPM);
//    points.emplace_back(15 / PPM, 50 / PPM);
//    points.emplace_back(5 / PPM, 55 / PPM);
//    points.emplace_back(-5 / PPM, 55 / PPM);
//    shape.Set(points.data(), (int)points.size());
//
//    b2FixtureDef fixtureDef;
//    fixtureDef.filter.categoryBits = CollisionCategory::PlayerEmitter;
//    fixtureDef.filter.maskBits = 0;//CollisionCategory::Asteroid | CollisionCategory::PlanetCore;
//    fixtureDef.density = 0.5;
//    fixtureDef.friction = 0.5;
//    fixtureDef.restitution = 0.2;
//    fixtureDef.shape = &shape;
//
//    emitter->CreateFixture(&fixtureDef);
//    
//    linkToFrame(emitter);
//}
//
//void Player::selectTool(PlayerToolType toolType)
//{
//    unselectTool();
//    
//    switch (toolType) {
//        case PlayerToolType::Collector: {
//            CollectorTool *collector = new CollectorTool(world->world, this, frame->GetPosition());
//            linkToFrame(collector->body);
//            tool = collector;
//            break;
//        }
//        case PlayerToolType::Shield: {
//            ShieldTool *shield = new ShieldTool(world->world, frame->GetPosition());
//            linkToFrame(shield->body);
//            tool = shield;
//            break;
//        }
//        default:
//            break;
//    }
//}
//
//void Player::unselectTool()
//{
//    if (!tool)
//        return;
//    world->world->DestroyBody(tool->getBody());
//}
//
//void Player::linkToFrame(b2Body *body)
//{
//    b2WeldJointDef cjd;
//    cjd.collideConnected = false;
//    cjd.bodyA = frame;
//    cjd.bodyB = body;
//    world->world->CreateJoint(&cjd);
//}
//
//void Player::move(sf::Vector2<float> vector)
//{
//    b2Vec2 imp;
//    imp.x = vector.x * frame->GetMass();
//    imp.y = vector.y * frame->GetMass();
//    
////    createLiquid(emitter->GetWorldCenter().x, emitter->GetWorldCenter().y, imp * -1, sf::Color::Blue);
//    
//    emitter->ApplyLinearImpulse(imp, emitter->GetWorldCenter(), true);
//    
//    // Limit max velocity
//    b2Vec2 vel = emitter->GetLinearVelocity();
//    if (abs(vel.Length()) > maxVelocity) {
//        vel = vel / vel.Length() * maxVelocity;
//        emitter->SetLinearVelocity(vel);
//    }
//}
//
//void Player::rotate(float desiredAngle)
//{
//    this->desiredAngle = desiredAngle + M_PI_2;
//}
//
//void Player::createLiquid(float x, float y, b2Vec2 initial_force, sf::Color color)
//{
////    b2ParticleDef def;
////    def.position = b2Vec2(x, y);
////    def.color = b2ParticleColor(color.r, color.g, color.b, color.a);
////    def.flags = b2_waterParticle | b2_viscousParticle;
////    int index = world.particle_system->CreateParticle(def);
////    world.particle_system->ApplyForce(index, index, initial_force);
//}
//
//void Player::step(float dt)
//{
//    float angle = emitter->GetAngle() + emitter->GetAngularVelocity() * dt;
//    float target = desiredAngle;
//    
//    float delta = fmod(target, M_PI * 2) - fmod(angle, M_PI * 2);
//
//    if (delta > M_PI_2)
//        delta -= M_PI * 2;
//    
//    if (delta <= -M_PI_2)
//        delta += M_PI * 2;
//    
//    float angvel = delta / dt;
//    float imp = emitter->GetInertia() * angvel;
//    
//    emitter->ApplyAngularImpulse(imp, true);
//}
//
//void Player::contactBegin(BaseEntity *entity, b2Fixture *fixture) {
//    if (!fixture)
//        return;
//    
//    b2Body *body = fixture->GetBody();
//    if (body == tool->getBody()) {
//        tool->collideWithEntity(entity);
//    } else {
//        BaseEntity::contactBegin(entity, fixture);
//    }
//}
//
//void Player::receiveCollision(BaseEntity *entity, float impulse)
//{
//    const std::vector<std::pair<float, float>> impactToDamage = {{5, 1}, {20, 2}, {50, 3}, {100, 10}};
//    
//    float damage = 0;
//    float impact = std::abs(impulse);// * speed;
//    
//    for (int i = 0; i < impactToDamage.size(); i++) {
//        float imp = impactToDamage[i].first;
//        if (impact < imp) {
//            if (i > 0) {
//                damage = impactToDamage[i - 1].second;
//                break;
//            } else {
//                break;
//            }
//        }
//        if (i == impactToDamage.size() - 1) {
//            damage = impactToDamage[i].second;
//        }
//    }
//    if (damage == 0) {
//        return;
//    }
//    
////    printf("IMAPCT: %2.2f, DAMAGE: %1.0f, HEALTH: %3.0f\n", std::abs(impulse), damage, health);
//    
//    health -= damage;
//}
//
//void Player::destroy() {}
//bool Player::isDestroying() { return false; }
//
//void Player::render(sf::RenderWindow *window, Camera camera)
//{
//    
//}
