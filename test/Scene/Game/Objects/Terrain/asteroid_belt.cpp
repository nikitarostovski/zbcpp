#include "asteroid_belt.hpp"
#include "polygon.hpp"
#include "solid_block.hpp"

using namespace PolygonUtils;

AsteroidBelt::AsteroidBelt(b2Vec2 pos, float startRadius, float endRadius, float asteroidSize, PhysicsWorld *world)
{
    float bradiusstart = startRadius + asteroidSize;
    float bradiusend = endRadius;
    
    Polygon bcontour = Polygon::makeCircle(pos, (bradiusend + asteroidSize), 32, MaterialType::blue, true);
    auto bchunks = bcontour.split(asteroidSize);
    
    std::vector<Polygon> asteroids;
    for (auto &bchunk : bchunks) {
        float dist = (pos - bchunk.center).Length();
        if (dist < bradiusstart || dist > bradiusend) {
            continue;
        }
        
        bool isValid = true;
        for (auto &a : asteroids) {
            float dd = (bchunk.center - a.center).Length();
            if (dd <= (2 * asteroidSize)) {
                isValid = false;
                break;
            }
        }
        if (isValid)
            asteroids.push_back(bchunk);
    }
    for (auto chunk: asteroids) {
        SolidBlock *block = new SolidBlock(chunk);
        world->addEntity(block);
    }
}

AsteroidBelt::~AsteroidBelt()
{

}

void AsteroidBelt::step(float dt)
{
    
}
