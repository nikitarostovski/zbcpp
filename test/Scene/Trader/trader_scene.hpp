#pragma once
#include "SFML/Graphics.hpp"
#include "base_scene.hpp"
#include "player.hpp"
#include "trader.hpp"

class TraderScene : public BaseScene
{
private:
    Player *player;
    Trader *trader;
    
    sf::Font mainFont;
    sf::RectangleShape background;
    sf::Text materialText;
    sf::Text healthText;
public:
    std::function<void()> onClose;
    
    TraderScene(sf::RenderWindow *window, Player *player, Trader *trader);
    
    void step(float dt) override;
    void render() override;
    
    void handleKeyDown(sf::Keyboard::Key key) override;
};
