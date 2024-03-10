#include "terrain_renderer.hpp"
#include "resource_path.hpp"
#include "string_utils.hpp"
#include "constants.h"

TerrainRenderer::TerrainRenderer(PolygonUtils::MaterialType type)
{
    vertices = sf::VertexArray(sf::Triangles);
    
    std::string name;
    switch (type) {
        case PolygonUtils::MaterialTypeRock:
            name = "rock.png";
            break;
        case PolygonUtils::MaterialTypeDirt:
            name = "dirt.png";
            break;
        case PolygonUtils::MaterialTypeCrystalRed:
            name = "crystal_red.png";
            break;
        case PolygonUtils::MaterialTypeCrystalGreen:
            name = "crystal_green.png";
            break;
        case PolygonUtils::MaterialTypeCore:
            name = "core.png";
            break;
        case PolygonUtils::MaterialTypeExplosive:
            name = "explosive.png";
            break;
        case PolygonUtils::MaterialTypeLava:
            name = "lava.png";
            break;
    }
    texture.loadFromFile(resourcePath() + name);
    texture.setSmooth(true);
    texture.setRepeated(true);
}

void TerrainRenderer::addVertex(sf::Vertex vertex)
{
    vertices.resize(vertices.getVertexCount() + 1);
    vertices[vertices.getVertexCount() - 1] = vertex;
}

void TerrainRenderer::addShape(std::vector<sf::Vertex> newVertices)
{
    auto initialSize = vertices.getVertexCount();
    vertices.resize(initialSize + newVertices.size());
    for (int i = 0; i < newVertices.size(); i++)
        vertices[initialSize + i] = newVertices[i];
}

void TerrainRenderer::reset()
{
    vertices.clear();
}

void TerrainRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = &texture;
    states.transform *= getTransform();
    target.draw(vertices, states);
}
