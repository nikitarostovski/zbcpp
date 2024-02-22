#pragma once
#include <vector>
#include "Box2D.h"
#include "base_entity.hpp"

class Chunk {
private:
    sf::RectangleShape rect;
public:
    std::vector<BaseEntity *> entities;
    
    Chunk();
    
    void removeDeadEntities(b2World *world);
    
    void render(sf::RenderWindow *window, Camera camera, std::pair<int, int> address);
};
