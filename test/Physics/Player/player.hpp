//#pragma once
//#include "Box2D.h"
//#include "entity.hpp"
//#include "SFML/Graphics.hpp"
//#include "physics_world.hpp"
//#include "player_tool.hpp"
//
//class Player : public BaseEntity
//{
//private:
//    float desiredAngle;
//    float maxVelocity;
//    
//    void createFrame(b2Vec2 pos);
//    void createEmitter();
//    void unselectTool();
//    void linkToFrame(b2Body *body);
//    
//    void createLiquid(float x, float y, b2Vec2 initial_force, sf::Color color);
//    
//public:
//    PhysicsWorld *world;
//    
//    b2Body *frame;
//    b2Body *emitter;
//    
//    PlayerTool *tool;
//    
//    float maxHealth;
//    float health;
//    
//    uint64_t collectedMaterialCount;
//    
//    Player(PhysicsWorld *world_, b2Vec2 pos);
//    
//    int getEntityType() override;
//    void contactBegin(BaseEntity *entity, b2Fixture *fixture) override;
//    void receiveCollision(BaseEntity *entity, float impulse) override;
//    b2Vec2 getSpeed() override;
//    void destroy() override;
//    bool isDestroying() override;
//    
//    void selectTool(PlayerToolType toolType);
//    
//    void move(sf::Vector2<float> vector);
//    void rotate(float desiredAngle);
//    
//    void step(float dt);
//    void render(sf::RenderWindow *window, Camera camera) override;
//};
