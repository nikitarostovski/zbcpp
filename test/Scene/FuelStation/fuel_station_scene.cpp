#include "fuel_station_scene.hpp"
#include "resource_path.hpp"
#include "imgui.h"
#include "string_utils.hpp"

FuelStationScene::FuelStationScene(sf::RenderWindow *window, Player *player, FuelStation *fuelStation)
    : BaseScene(window)
    , player(player)
    , fuelStation(fuelStation)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    
    background = sf::RectangleShape();
    background.setFillColor(sf::Color(64, 64, 128, 240));
}

void FuelStationScene::handleKeyDown(sf::Keyboard::Key key)
{
    if ((key == sf::Keyboard::Tab || key == sf::Keyboard::Escape) && onClose) {
        onClose();
    }
    BaseScene::handleKeyDown(key);
}

void FuelStationScene::step(float dt)
{
    BaseScene::step(dt);
    
    int windowFlags =
        ImGuiWindowFlags_::ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 size = ImVec2(window->getSize().x - 2 * 64, window->getSize().y - 2 * 64);
    
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Fuel Station", 0, windowFlags);
    
    std::string material = "Material: " + std::to_string(player->config.material);
    ImGui::Text("%s", material.c_str());
    
    std::string fuel = "Fuel: " + std::to_string(player->ship->shipConfig.fuel);
    ImGui::Text("%s", fuel.c_str());
    
    ImGui::Text("--- Available ---");
    
    std::string fuelAvailable = "Available fuel: " + std::to_string(fuelStation->config.fuelAmount);
    ImGui::Text("%s", fuelAvailable.c_str());
    
    
    float amount1 = 1.0f;
    std::string amount1Text = string_format("Fuel: %.0f for %.2f material", amount1, amount1 * fuelStation->config.fuelPrice);
    if (ImGui::Button(amount1Text.c_str(), ImVec2{400.0f, 50.0f})) {
        if (player->config.material >= fuelStation->config.fuelPrice * amount1) {
            this->player->config.material -= fuelStation->config.fuelPrice * amount1;
            this->fuelStation->config.fuelAmount -= amount1;
            this->player->ship->shipConfig.fuel += amount1;
        }
    }
    
    float amount10 = 10.0f;
    std::string amount10Text = string_format("Fuel: %.0f for %.2f material", amount10, amount10 * fuelStation->config.fuelPrice);
    if (ImGui::Button(amount10Text.c_str(), ImVec2{400.0f, 50.0f})) {
        if (player->config.material >= fuelStation->config.fuelPrice * amount10) {
            this->player->config.material -= fuelStation->config.fuelPrice * amount10;
            this->fuelStation->config.fuelAmount -= amount10;
            this->player->ship->shipConfig.fuel += amount10;
        }
    }
    
    float amount100 = 100.0f;
    std::string amount100Text = string_format("Fuel: %.0f for %.2f material", amount100, amount100 * fuelStation->config.fuelPrice);
    if (ImGui::Button(amount100Text.c_str(), ImVec2{400.0f, 50.0f})) {
        if (player->config.material >= fuelStation->config.fuelPrice * amount100) {
            this->player->config.material -= fuelStation->config.fuelPrice * amount100;
            this->fuelStation->config.fuelAmount -= amount100;
            this->player->ship->shipConfig.fuel += amount100;
        }
    }
    
    ImGui::End();
}

void FuelStationScene::render()
{
    const unsigned int margin = 64;
    auto backgroundSize = window->getSize();
    backgroundSize.x -= 2 * margin;
    backgroundSize.y -= 2 * margin;
    background.setSize(sf::Vector2f(backgroundSize.x, backgroundSize.y));
    background.setPosition(margin, margin);
    
    window->draw(background);
    
    BaseScene::render();
}
