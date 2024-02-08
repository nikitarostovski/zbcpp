#pragma once
#include "SFML/Graphics.hpp"

class BaseScene;
class Player;
class Trader;
class FuelStation;

class Game
{
private:
    sf::RenderWindow* window;
    std::vector<BaseScene *> visibleScenes;
    
    void showMainMenu();
    void showGame();
    void showInventory(Player *player);
    void showTrader(Player *player, Trader *trader);
    void showFuelStation(Player *player, FuelStation *fuelStation);
    void quit();
public:
    Game(sf::RenderWindow* window);
    
    std::function<void()> onQuit;
    
    void handleEvent(sf::Event event);
    void step(float dt);
};
