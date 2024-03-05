#pragma once
#include "SFML/Graphics.hpp"
#include "camera.hpp"

class TerrainRenderer : public sf::Drawable, public sf::Transformable
{
private:
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
public:
    TerrainRenderer();
    
    void addVertex(sf::Vertex vertex);
    void addShape(std::vector<sf::Vertex> vertices);
    void reset();
};
