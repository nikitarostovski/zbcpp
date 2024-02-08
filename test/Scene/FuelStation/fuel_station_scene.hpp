#pragma once
#include "SFML/Graphics.hpp"
#include "base_scene.hpp"
#include "player.hpp"
#include "fuel_station.hpp"

class FuelStationScene : public BaseScene
{
private:
    Player *player;
    FuelStation *fuelStation;
    
    sf::RectangleShape background;
public:
    std::function<void()> onClose;
    
    FuelStationScene(sf::RenderWindow *window, Player *player, FuelStation *fuelStation);
    
    void step(float dt) override;
    void render() override;
    
    void handleKeyDown(sf::Keyboard::Key key) override;
};
