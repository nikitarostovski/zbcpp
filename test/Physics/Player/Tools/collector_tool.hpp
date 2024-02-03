//#pragma once
//#include "constants.h"
//#include "player_tool.hpp"
//
//class CollectorTool : public PlayerTool
//{
//private:
//    Player *parent;
//    
//public:
//    b2Body *body;
//    
//    CollectorTool(b2World *world, Player *player, b2Vec2 pos)
//    {
//        parent = player;
//        
//        b2BodyDef boxBodyDef;
//        boxBodyDef.position.Set(pos.x, pos.y);
//        boxBodyDef.type = b2_dynamicBody;
//        body = world->CreateBody(&boxBodyDef);
//        body->SetUserData(player);
//        
//        b2PolygonShape shape;
//        std::vector<b2Vec2> points;
//        points.emplace_back(10 / PPM, -10 / PPM);
//        points.emplace_back(-10 / PPM, -10 / PPM);
//        points.emplace_back(-15 / PPM, 0);
//        points.emplace_back(15 / PPM, 0);
//        shape.Set(points.data(), (int)points.size());
//
//        b2FixtureDef fixtureDef;
//        fixtureDef.filter.categoryBits = CollisionCategory::PlayerCollector;
//        fixtureDef.filter.maskBits = CollisionCategory::Asteroid;
//        fixtureDef.isSensor = true;
//        fixtureDef.density = 0.5;
//        fixtureDef.friction = 0.5;
//        fixtureDef.restitution = 0.2;
//        fixtureDef.shape = &shape;
//
//        body->CreateFixture(&fixtureDef);
//    }
//    
//    PlayerToolType getType() override 
//    {
//        return PlayerToolType::Collector;
//    }
//    
//    b2Body* getBody() override 
//    {
//        return body;
//    }
//    
//    void collideWithEntity(BaseEntity *entity) override
//    {
//        if (!entity)
//            return;
//        
//        if (entity->getEntityType() == 2) {
//            parent->collectedMaterialCount++;
//            entity->destroy();
//        }
//    }
//};
