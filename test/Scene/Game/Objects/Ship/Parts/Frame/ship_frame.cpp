#include "ship_frame.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipFrame::ShipFrame(b2Vec2 pos, FrameConfig config, sf::Color color)
    : BaseEntity(false)
    , config(config)
    , pos(pos)
    , color(color)
{ }

CollisionCategory ShipFrame::getEntityType()
{
    return CollisionCategory::PlayerFrame;
}

void ShipFrame::render(sf::RenderWindow *window, Camera camera)
{
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2Fixture *fixture = body->GetFixtureList();
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(color);
    polygon.setOrigin(localCenter);
    polygon.setPosition((worldCenter.x - camera.x) * PPM + window->getSize().x / 2,
                        (worldCenter.y - camera.y) * PPM + window->getSize().y / 2);
    polygon.setPointCount(shape->GetVertexCount());
    for (int i = 0; i < shape->GetVertexCount(); i++) {
        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * PPM, shape->GetVertex(i).y * PPM));
    }
    polygon.setRotation(angle * DEG_PER_RAD);
    window->draw(polygon);
}
