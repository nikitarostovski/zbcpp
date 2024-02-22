#pragma once
#include "ship_frame.hpp"

using namespace PolygonUtils;

class ShipFrameBasic : public ShipFrame {
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    ShipFrameBasic(b2Vec2 pos, FrameConfig config);
    
    void receiveCollision(BodyEntity *entity, float impulse) override;
};
