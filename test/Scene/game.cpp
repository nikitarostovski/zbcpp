#include "game.hpp"
#include "base_scene.hpp"
#include "main_menu_scene.hpp"
#include "game_scene.hpp"
#include "inventory_scene.hpp"
#include "trader_scene.hpp"
#include "fuel_station_scene.hpp"

Game::Game(sf::RenderWindow* window)
{
    this->window = window;
    showMainMenu();
}

void Game::showMainMenu() {
    visibleScenes.clear();
    auto scene = new MainMenuScene(window);
    scene->onPlay = std::bind(&Game::showGame, this);
    scene->onQuit = std::bind(&Game::quit, this);
    visibleScenes.push_back(scene);
}

void Game::showGame() {
    visibleScenes.clear();
    auto scene = new GameScene(window);
    scene->onShowInventory = std::bind(&Game::showInventory, this, std::placeholders::_1);
    scene->onShowTrader = std::bind(&Game::showTrader, this, std::placeholders::_1, std::placeholders::_2);
    scene->onShowFuelStation = std::bind(&Game::showFuelStation, this, std::placeholders::_1, std::placeholders::_2);
        
    visibleScenes.push_back(scene);
    scene->isPaused = false;
}

void Game::showInventory(Player *player) {
    GameScene *gameScene = static_cast<GameScene *>(visibleScenes[visibleScenes.size() - 1]);
    if (gameScene)
        gameScene->isPaused = true;
    
    auto scene = new InventoryScene(window, player);
    scene->onClose = [this, gameScene]{
        visibleScenes.pop_back();
        gameScene->isPaused = false;
    };
    visibleScenes.push_back(scene);
}

void Game::showTrader(Player *player, Trader *trader) {
    GameScene *gameScene = static_cast<GameScene *>(visibleScenes[visibleScenes.size() - 1]);
    if (gameScene)
        gameScene->isPaused = true;
    
    auto scene = new TraderScene(window, player, trader);
    scene->onClose = [this, gameScene]{
        visibleScenes.pop_back();
        gameScene->isPaused = false;
    };
    visibleScenes.push_back(scene);
}

void Game::showFuelStation(Player *player, FuelStation *fuelStation)
{
    GameScene *gameScene = static_cast<GameScene *>(visibleScenes[visibleScenes.size() - 1]);
    if (gameScene)
        gameScene->isPaused = true;
    
    auto scene = new FuelStationScene(window, player, fuelStation);
    scene->onClose = [this, gameScene]{
        visibleScenes.pop_back();
        gameScene->isPaused = false;
    };
    visibleScenes.push_back(scene);
}

void Game::quit() {
    onQuit();
}

void Game::handleEvent(sf::Event event) {
    if (visibleScenes.empty())
        return;
    visibleScenes[visibleScenes.size() - 1]->handleEvent(event);
}

void Game::step(float dt)
{
    window->clear();
    for (auto scene: visibleScenes) {
        scene->step(dt);
        scene->render();
    }
    window->display();
}
