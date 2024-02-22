#include "chunk.hpp"
#include "body_entity.hpp"
#include "constants.h"

Chunk::Chunk()
    : entities(std::vector<BaseEntity *>())
{
    rect = sf::RectangleShape();
    rect.setFillColor(sf::Color(255, 0, 0, 32));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1.0f);
}

void Chunk::removeDeadEntities(b2World *world)
{
    for (int i = 0; i < entities.size();) {
        BaseEntity *entity = entities[i];
        if (entity && entity->isDead) {
            entities.erase(entities.begin() + i);
            entity->deactivate(world);
//            delete entity;
        } else {
            i++;
        }
    }
}

void Chunk::render(sf::RenderWindow *window, Camera camera, std::pair<int, int> address)
{
    float xc = (address.first * CHUNK_SIZE - camera.x) * camera.scale + window->getSize().x / 2;
    float yc = (address.second * CHUNK_SIZE - camera.y) * camera.scale + window->getSize().y / 2;
    
    rect.setSize(sf::Vector2f{CHUNK_SIZE * camera.scale, CHUNK_SIZE * camera.scale});
    rect.setPosition(xc, yc);
    
    window->draw(rect);
}
