#pragma once
#include "ship_frame.hpp"

using namespace PolygonUtils;

class ShipFrameBasic : public ShipFrame {
public:
    ShipFrameBasic(b2Vec2 pos, FrameConfig config);
    
    void initializeBody(b2World *world) override;
    void receiveCollision(BaseEntity *entity, float impulse) override;
};
