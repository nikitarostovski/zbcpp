#pragma once
#include "ship_frame.hpp"

using namespace PolygonUtils;

class ShipFrameAdvanced : public ShipFrame {
protected:
    b2Body* createBody(b2World *world) override;
    b2AABB getInitialAABB() override;
public:
    ShipFrameAdvanced(b2Vec2 pos, FrameConfig config);
    
    void receiveCollision(BodyEntity *entity, float impulse) override;
};
