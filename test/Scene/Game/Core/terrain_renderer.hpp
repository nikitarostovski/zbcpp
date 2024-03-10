#pragma once
#include "SFML/Graphics.hpp"
#include "camera.hpp"
#include "material.hpp"

class TerrainRenderer : public sf::Drawable, public sf::Transformable
{
private:
    sf::Texture texture;
    
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
public:
    TerrainRenderer(PolygonUtils::MaterialType type);
    
    void addVertex(sf::Vertex vertex);
    void addShape(std::vector<sf::Vertex> vertices);
    void reset();
};
