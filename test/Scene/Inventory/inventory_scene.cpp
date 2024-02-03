#include "inventory_scene.hpp"
#include "resource_path.hpp"
#include "imgui.h"
#include "string_utils.hpp"

InventoryScene::InventoryScene(sf::RenderWindow *window, Player *player)
    : BaseScene(window)
    , player(player)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    
    background = sf::RectangleShape();
    background.setFillColor(sf::Color(64, 64, 64, 240));
}

void InventoryScene::handleKeyDown(sf::Keyboard::Key key)
{
    if ((key == sf::Keyboard::Tab || key == sf::Keyboard::Escape) && onClose) {
        onClose();
    }
    BaseScene::handleKeyDown(key);
}

void InventoryScene::step(float dt)
{
    BaseScene::step(dt);
    
    int windowFlags =
        ImGuiWindowFlags_::ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 size = ImVec2(300, 200);
    
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Inventory", 0, windowFlags);
    
    std::string health = "Health: " + std::to_string((int)player->config.health);
    ImGui::Text("%s", health.c_str());
    
    std::string material = "Material: " + std::to_string(player->config.material);
    ImGui::Text("%s", material.c_str());
    
    ImGui::Text("--- Current ship ---");
    
    std::string frameName = frameTypeString(player->ship->shipConfig.frame.category);
    int frameArmor = 100 * player->ship->shipConfig.frame.armor;
    std::string shipFrame = string_format("Frame: \"%s\". Armor: %d\%", frameName.c_str(), frameArmor);
    ImGui::Text("%s", shipFrame.c_str());
    
    std::string shipEmitter = string_format("Emitter \"%s\". Max speed: %.0f Acceleration: %.1f", emitterTypeString(player->ship->shipConfig.emitter.category).c_str(), player->ship->shipConfig.emitter.maxSpeed, player->ship->shipConfig.emitter.acceleration);
    ImGui::Text("%s", shipEmitter.c_str());
    
    std::string shipCollector = string_format("Collector: \"%s\". Capacity: %.0f", collectorTypeString(player->ship->shipConfig.collector.category).c_str(), player->ship->shipConfig.collector.capacity);
    ImGui::Text("%s", shipCollector.c_str());
    
    ImGui::End();
}

void InventoryScene::render()
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
