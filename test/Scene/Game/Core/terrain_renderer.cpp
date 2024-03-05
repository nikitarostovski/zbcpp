#include "terrain_renderer.hpp"

TerrainRenderer::TerrainRenderer()
{
    vertices = sf::VertexArray(sf::Triangles);
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
    states.transform *= getTransform();
    target.draw(vertices, states);
}
