#pragma once
#include "SFML/Graphics.hpp"
#include "base_scene.hpp"

class MainMenuScene : public BaseScene
{
private:
    void onPlayClick();
    void onQuitClick();
public:
    MainMenuScene(sf::RenderWindow *window);
    ~MainMenuScene();
    
    std::function<void()> onPlay;
    std::function<void()> onQuit;
    
    void step(float dt) override;
    void render() override;
};
