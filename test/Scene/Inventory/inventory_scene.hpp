#pragma once
#include "SFML/Graphics.hpp"
#include "base_scene.hpp"
#include "player.hpp"

class InventoryScene : public BaseScene
{
private:
    Player *player;
    
    sf::Font mainFont;
    sf::RectangleShape background;
    sf::Text materialText;
    sf::Text healthText;
public:
    std::function<void()> onClose;
    
    InventoryScene(sf::RenderWindow *window, Player *player);
    
    void step(float dt) override;
    void render() override;
    
    void handleKeyDown(sf::Keyboard::Key key) override;
};
