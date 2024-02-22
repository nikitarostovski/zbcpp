#include "base_entity.hpp"
#include "constants.h"

BaseEntity::BaseEntity()
{
    this->lastChunkXLo = 0;
    this->lastChunkXHi = 0;
    this->lastChunkYLo = 0;
    this->lastChunkYHi = 0;
}

BaseEntity::~BaseEntity()
{ }

bool BaseEntity::checkIfChunkChanged()
{
    auto aabb = getAABB();

    int xLo = static_cast<int>(std::floor(aabb.lowerBound.x / CHUNK_SIZE));
    int xHi = static_cast<int>(std::floor(aabb.upperBound.x / CHUNK_SIZE));
    int yLo = static_cast<int>(std::floor(aabb.lowerBound.y / CHUNK_SIZE));
    int yHi = static_cast<int>(std::floor(aabb.upperBound.y / CHUNK_SIZE));
        
    bool positionChanged = xLo != lastChunkXLo || xHi != lastChunkXHi || yLo != lastChunkYLo || yHi != lastChunkYHi;
    
    this->lastChunkXLo = xLo;
    this->lastChunkXHi = xHi;
    this->lastChunkYLo = yLo;
    this->lastChunkYHi = yHi;
    
    return positionChanged;
}

std::string BaseEntity::getName()
{
    return "unnamed";
}
