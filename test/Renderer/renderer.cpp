//#pragma once
//#include "renderer.hpp"
//#include "constants.h"
//#include "resource_path.hpp"
//
//void renderBody(b2Body *body, sf::Color color, Camera camera, sf::RenderWindow& window);
//void renderBlock(SolidBlock *block, Camera camera, sf::RenderWindow& window);
//
//Renderer::Renderer(Scene *scene_, sf::RenderWindow &window)
//    : m_scene(scene_)
//    , m_window(window)
//{
//    camera = Camera{0, 0, 1};
//    mainFont.loadFromFile(resourcePath() + "Signwood-Regular.ttf");
//}
//
//void Renderer::render()
//{
//    m_window.clear();
//    
//    camera.x = m_scene->player->frame->GetWorldCenter().x;
//    camera.y = m_scene->player->frame->GetWorldCenter().y;
//    
//    drawPlanets();
//    drawBlocks();
//    drawLiquid();
//    drawPlayer();
//    
//    m_window.display();
//}
//
//void Renderer::drawPlanets()
//{
//    for (Planet *planet : m_scene->planets) {
//        b2Vec2 center = b2Vec2(planet->center.x, planet->center.y);
//        for (auto coreChunk: planet->core) {
//            renderBlock(coreChunk, camera, m_window);
//        }
//        float gravity_radius = planet->gravity_radius;
//        sf::CircleShape gravity_field;
//        gravity_field.setOrigin(sf::Vector2<float>(gravity_radius * PPM, gravity_radius * PPM));
//        gravity_field.setPosition((center.x - camera.x) * PPM + m_window.getSize().x / 2, (center.y - camera.y) * PPM + m_window.getSize().y / 2);
//        gravity_field.setRadius(gravity_radius * PPM);
//        gravity_field.setFillColor(sf::Color::Transparent);
//        gravity_field.setOutlineColor(sf::Color::White);
//        gravity_field.setOutlineThickness(2);
//        m_window.draw(gravity_field);
//    }
//}
//
//void Renderer::drawBlocks()
//{
//    for (auto block : m_scene->blocks) {
//        renderBlock(block, camera, m_window);
//    }
//}
//
//void Renderer::drawPlayer()
//{
//    Player *player = m_scene->player;
//    
//    renderBody(player->frame, sf::Color::Yellow, camera, m_window);
//    renderBody(player->emitter, sf::Color::Red, camera, m_window);
//    
//    if (player->tool) {
//        sf::Color color = sf::Color::Transparent;
//        switch (player->tool->getType()) {
//            case PlayerToolType::Collector:
//                color = sf::Color::Cyan;
//                break;
//            case PlayerToolType::Shield:
//                color = sf::Color::Red;
//                break;
//            default:
//                break;
//        }
//        renderBody(player->tool->getBody(), color, camera, m_window);
//    }
//    
//    float x = (player->frame->GetWorldCenter().x - camera.x) * PPM + m_window.getSize().x / 2;
//    float y = (player->frame->GetWorldCenter().y - camera.y) * PPM + m_window.getSize().y / 2;
//    
//    std::string healthText = std::to_string((int)player->health);
//    drawText(healthText, x, y - 8, sf::Color::Green);
//    
//    std::string materialText = std::to_string((int)player->collectedMaterialCount);
//    drawText(materialText, x, y + 8, sf::Color::Yellow);
//}
//
//void Renderer::drawText(std::string text, float x, float y, sf::Color color, int size)
//{
//    sf::Text label;
//    label.setPosition(x, y);
//    label.setFont(mainFont);
//    label.setString(text);
//    label.setCharacterSize(size);
//    label.setFillColor(color);
//    label.setOutlineColor(sf::Color::Red);
//    label.setOutlineThickness(1);
//    label.setOrigin(label.getGlobalBounds().width / 2, label.getGlobalBounds().height / 2);
//    m_window.draw(label);
//}
//
//void Renderer::drawLiquid()
//{
//    int count = m_scene->particle_system->GetParticleCount();
//    
//    float radius = m_scene->particle_system->GetRadius();
//    b2Vec2 *positions = m_scene->particle_system->GetPositionBuffer();
//    b2ParticleColor *colors = m_scene->particle_system->GetColorBuffer();
//    
//    for (int i = 0; i < count; i++)
//    {
//        auto pos = positions[i];
//        auto col = colors[i];
//        
//        sf::CircleShape particle;
//        particle.setOrigin(radius / 2 * PPM, radius / 2 * PPM);
//        particle.setPosition((pos.x - camera.x) * PPM + m_window.getSize().x / 2, (pos.y - camera.y) * PPM + m_window.getSize().y / 2);
//        particle.setRadius(radius * PPM);
//        particle.setFillColor(sf::Color(col.r, col.g, col.b));
//        
//        m_window.draw(particle);
//    }
//}
//
//
//void renderBlock(SolidBlock *block, Camera camera, sf::RenderWindow& window)
//{
//    if (block->body == NULL)
//        return;
//    
//    for (b2Fixture *f = block->body->GetFixtureList(); f; f = f->GetNext()) {
//        b2Shape::Type shapeType = f->GetType();
//        if (shapeType == b2Shape::e_polygon) {
//            b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
//            sf::ConvexShape polygon;
//            
//            polygon.setOrigin(sf::Vector2<float>(block->body->GetLocalCenter().x * PPM, block->body->GetLocalCenter().y * PPM));
//            polygon.setPosition((block->body->GetWorldCenter().x - camera.x) * PPM + window.getSize().x / 2, (block->body->GetWorldCenter().y - camera.y) * PPM + window.getSize().y / 2);
//            polygon.setPointCount(shape->GetVertexCount());
//            for (int i = 0; i < shape->GetVertexCount(); i++) {
//                polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * PPM, shape->GetVertex(i).y * PPM));
//            }
//            polygon.setRotation(block->body->GetAngle() * DEG_PER_RAD);
//            
//            if (block->canBeCollected) {
//                polygon.setScale(1.1f, 1.1f);
//                polygon.setFillColor(sf::Color::Green);
//                polygon.setOutlineColor(sf::Color::Transparent);
//                window.draw(polygon);
//            }
//            
//            polygon.setScale(1.0f, 1.0f);
//            polygon.setFillColor(block->color);
//            window.draw(polygon);
//        }
//    }
//}
//
//void renderBody(b2Body *body, sf::Color color, Camera camera, sf::RenderWindow& window)
//{
//    sf::Vector2<float> localCenter{body->GetLocalCenter().x * PPM, body->GetLocalCenter().y * PPM};
//    sf::Vector2<float> worldCenter{body->GetWorldCenter().x, body->GetWorldCenter().y};
//    float angle = body->GetAngle();
//    
//    b2Fixture *fixture = body->GetFixtureList();
//    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
//    sf::ConvexShape polygon;
//    polygon.setFillColor(color);
//    polygon.setOrigin(localCenter);
//    polygon.setPosition((worldCenter.x - camera.x) * PPM + window.getSize().x / 2, (worldCenter.y - camera.y) * PPM + window.getSize().y / 2);
//    polygon.setPointCount(shape->GetVertexCount());
//    for (int i = 0; i < shape->GetVertexCount(); i++) {
//        polygon.setPoint(i, sf::Vector2<float>(shape->GetVertex(i).x * PPM, shape->GetVertex(i).y * PPM));
//    }
//    polygon.setRotation(angle * DEG_PER_RAD);
//    window.draw(polygon);
//}
