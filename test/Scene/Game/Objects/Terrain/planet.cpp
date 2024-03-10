#include "planet.hpp"
#include "polygon.hpp"
#include "solid_block.hpp"
#include "gravity_field_entity.hpp"

using namespace PolygonUtils;

Planet::Planet(b2Vec2 pos, float radius, float gravityRadius, PhysicsWorld *world)
{
    this->center = pos;
    this->gravityRadius = gravityRadius;
    
    Polygon corePolygon = Polygon::makeCircle(pos, radius, 8, MaterialTypeCore, false, true);
    SolidBlock *coreBlock = new SolidBlock(corePolygon, true, world);
    world->addEntity(coreBlock);
    
    GravityFieldEntity *field = new GravityFieldEntity(pos, gravityRadius, 0.005f, world);
    world->addEntity(field);
}

Planet::~Planet()
{

}

void Planet::step(float dt)
{
    
}
