#include <iostream>
#include "base_scene.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

BaseScene::BaseScene(sf::RenderWindow *window)
{
    this->window = window;
    if (!ImGui::SFML::Init(*window))
        printf("Error loading gui tools\n");
    deltaClock = sf::Clock();
}

void BaseScene::handleEvent(sf::Event event)
{
    if (!window)
        return;
    
    ImGui::SFML::ProcessEvent(*window, event);
    switch (event.type) {
        case sf::Event::KeyPressed:
            handleKeyDown(event.key.code);
            break;
        case sf::Event::KeyReleased:
            handleKeyUp(event.key.code);
            break;
        case sf::Event::MouseMoved: {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);
            handleMouseMove(pos);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);
            handleMouseDown(pos);
            break;
        }
        case sf::Event::MouseButtonReleased: {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);
            handleMouseUp(pos);
            break;
        }
        default:
            break;
    }
}

void BaseScene::render()
{
    ImGui::SFML::Render(*window);
}

void BaseScene::step(float dt)
{
    ImGui::SFML::Update(*window, deltaClock.restart());
}
