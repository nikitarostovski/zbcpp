#pragma once
#include "controlled_ship.hpp"
#include "player_config.hpp"

class Player
{
private:
    sf::Font mainFont;
    
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    
    sf::RectangleShape fuelBar;
    sf::RectangleShape fuelBarBackground;
    
    sf::Text materialText;
    
public:
    PlayerConfig config;
    ControlledShip *ship;
    
    Player(b2Vec2 pos, PlayerConfig playerConfig, ShipConfig shipConfig, PhysicsWorld *world);
    ~Player();
    
    void render(sf::RenderWindow *window, Camera camera);
};
