#pragma once
#include "Box2D.h"
#include "base_entity.hpp"

enum PlayerToolType {
    None,
    Shield,
    Collector
};

class PlayerTool : public BaseEntity
{
public:
    virtual b2Body* getBody() { return nullptr; };
    virtual PlayerToolType getType() { return PlayerToolType::None; };
    
    virtual void collideWithEntity(BaseEntity *entity) {};
};
