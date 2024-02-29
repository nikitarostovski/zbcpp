#include "ship_frame.hpp"
#include "constants.h"
#include "collision_category.hpp"

ShipFrame::ShipFrame(FrameConfig config, b2Body *body)
    : config(config)
    , color(sf::Color(255, 255, 0))
{
    std::vector<b2Vec2> points;
    points.emplace_back(-2, 0);
    points.emplace_back(-3, 3);
    points.emplace_back(-1.5, 5);
    points.emplace_back(1.5, 5);
    points.emplace_back(3, 3);
    points.emplace_back(2, 0);
    
    b2PolygonShape shape;
    shape.Set(points.data(), (int)points.size());

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = CategoryShip;
    fixtureDef.filter.maskBits = CategoryShip | CategoryTerrain | CategoryTerrain | CategoryOrb;
    fixtureDef.density = config.mass;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 0.2;
    fixtureDef.shape = &shape;

    fixture = body->CreateFixture(&fixtureDef);
}

b2Fixture* ShipFrame::getFixture()
{
    return fixture;
}

void ShipFrame::renderFixture(sf::RenderWindow *window, Camera camera)
{
    if (!fixture)
        return;
    
    b2Body *body = fixture->GetBody();
    
    sf::Vector2<float> localCenter{body->GetLocalCenter().x * camera.scale, body->GetLocalCenter().y * camera.scale};
    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
    float angle = body->GetAngle();

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
