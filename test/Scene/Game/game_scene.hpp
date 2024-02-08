#pragma once
#include "SFML/Graphics.hpp"
#include "physics_world.hpp"
#include "base_scene.hpp"
#include "player.hpp"
#include "planet.hpp"
#include "asteroid_belt.hpp"
#include "trader.hpp"
#include "fuel_station.hpp"

class GameScene : public BaseScene
{
private:
    b2Vec2 playerMovement;
    float playerRotation;
    
    Camera camera;
    PhysicsWorld *world;
    
    Trader *activeTrader;
    FuelStation *activeFuelStation;
    
    Player *player;
    Trader *trader;
    FuelStation *fuelStation;
    Planet *mainPlanet;
    AsteroidBelt *mainAsteroids;
    
public:
    bool isPaused;
    std::function<void(Player *player)> onShowInventory;
    std::function<void(Player *player, Trader *trader)> onShowTrader;
    std::function<void(Player *player, FuelStation *fuelStation)> onShowFuelStation;
    
    GameScene(sf::RenderWindow *window);
    ~GameScene();
    
    void step(float dt) override;
    void render() override;
    
    void handleKeyDown(sf::Keyboard::Key key) override;
    void handleKeyUp(sf::Keyboard::Key key) override;
    void handleMouseMove(sf::Vector2i screenPos) override;
    void handleMouseDown(sf::Vector2i screenPos) override;
    void handleMouseUp(sf::Vector2i screenPos) override;
};
