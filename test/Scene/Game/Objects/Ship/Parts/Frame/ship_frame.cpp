#include "ship_frame.hpp"
#include "constants.h"

using namespace PolygonUtils;

ShipFrame::ShipFrame(b2Vec2 pos, FrameConfig config, sf::Color color)
    : BodyEntity(pos, PlayerFrame, false)
    , config(config)
    , pos(pos)
    , color(color)
{ }


void ShipFrame::render(sf::RenderWindow *window, Camera camera)
{
    if (!body)
        return;
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

    b2Fixture *fixture = body->GetFixtureList();
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    sf::ConvexShape polygon;
    polygon.setFillColor(color);
    polygon.setOrigin(localCenter);
    polygon.setPosition((worldCenter.x - camera.x) * camera.scale + window->getSize().x / 2,
                        (worldCenter.y - camera.y) * camera.scale + window->getSize().y / 2);
    polygon.setPointCount(shape->GetVertexCount());
    for (int i = 0; i < shape->GetVertexCount(); i++) {
        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * camera.scale, shape->GetVertex(i).y * camera.scale));
    }
    polygon.setRotation(angle * DEG_PER_RAD);
    window->draw(polygon);
}
