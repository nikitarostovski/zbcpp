#include "planet.hpp"
#include "polygon.hpp"
#include "solid_block.hpp"
#include "gravity_field.hpp"

using namespace PolygonUtils;

Planet::Planet(b2Vec2 pos, float radius, float gravityRadius, PhysicsWorld *world)
{
    this->center = pos;
    this->gravityRadius = gravityRadius;
    
    Polygon corePolygon = Polygon::makeCircle(pos, radius, 16, MaterialType::yellow, false);
    SolidBlock *coreBlock = new SolidBlock(corePolygon);
    world->addEntity(coreBlock);
    
    GravityField *field = new GravityField{pos.x, pos.y, gravityRadius, 0.1f};
    world->addGravityField(field);
}

Planet::~Planet()
{

}

void Planet::step(float dt)
{
    
}
