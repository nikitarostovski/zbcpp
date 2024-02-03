#include "main_menu_scene.hpp"
#include "imgui.h"

MainMenuScene::MainMenuScene(sf::RenderWindow *window)
    : BaseScene(window)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
}

void MainMenuScene::step(float dt)
{
    BaseScene::step(dt);
    
    int windowFlags =
        ImGuiWindowFlags_::ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 size = ImVec2(300, 200);
    
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Main menu", 0, windowFlags);
    
    if (ImGui::Button("Play", ImVec2(300, 100))) {
        ImGui::End();
        onPlayClick();
        return;
    }
    if (ImGui::Button("Quit", ImVec2(300, 100))) {
        ImGui::End();
        onQuitClick();
        return;
    }
    ImGui::End();
}

void MainMenuScene::render()
{
    BaseScene::render();
}

void MainMenuScene::onPlayClick()
{
    onPlay();
}

void MainMenuScene::onQuitClick()
{
    onQuit();
}
