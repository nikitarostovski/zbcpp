#include "game_scene.hpp"
#include "constants.h"
#include "string_utils.hpp"

b2Vec2 normalize(const b2Vec2 source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return b2Vec2(source.x / length, source.y / length);
    else
        return source;
}


GameScene::GameScene(sf::RenderWindow *window)
    :BaseScene(window)
{
    isPaused = false;
    
    camera.x = 0;
    camera.y = 0;
    camera.scale = 1;
    
    world = new PhysicsWorld();
    
    PlayerConfig playerConfig{20.0f, 20.0f, 350};
    FrameConfig frameConfig{FrameBasic, 0.0f, 1.0f};
    EmitterConfig emitterConfig{EmitterBasic, 10.0f, 0.5f, 1.0f};
    CollectorConfig collectorConfig{CollectorBasic, 10};
    ShipConfig shipConfig{frameConfig, emitterConfig, collectorConfig, 900.0f, 1000.0f};
    player = new Player(b2Vec2(0, 200), playerConfig, shipConfig, world);
    
    mainPlanet = new Planet(b2Vec2(0, 0), 100, 400, world);
    mainAsteroids = new AsteroidBelt(mainPlanet->center, mainPlanet->gravityRadius, mainPlanet->gravityRadius + 100, 20, world);
    
    std::vector<TradingItem> items;
    for (int i = 0; i < 3; i++) {
        FrameConfig newFrame = FrameConfig(FrameBasic, (static_cast<float>(i) / 3), 1.0f);
        int price = 30 + i * 7;
        std::string frameName = frameTypeString(newFrame.category);
        int frameArmor = 100 * newFrame.armor;
        
        std::string title = string_format("Frame \"%s\"", frameName.c_str());
        std::string description = string_format("Armor: %d\%\nPrice: %d", frameArmor, price);
        auto item = TradingItem(price, title, description, [newFrame, this](PlayerConfig *playerConfig, ShipConfig *shipConfig){
            shipConfig->frame = newFrame;
            this->player->ship->onConfigDidUpdate();
        });
        items.push_back(item);
    }
    for (int i = 0; i < 3; i++) {
        EmitterConfig newEmitter = EmitterConfig(EmitterBasic, 15 + i * 2, 0.5 + (static_cast<float>(i) / 2), 1.0f + 0.05f * i);
        int price = 20 + i * 5;
        
        std::string title = string_format("Emitter \"%s\"", emitterTypeString(newEmitter.category).c_str());
        std::string description = string_format("Max speed: %.0f Acceleration: %.1f\nPrice: %d", newEmitter.maxSpeed, newEmitter.acceleration, price);
        auto item = TradingItem(price, title, description, [newEmitter, this](PlayerConfig *playerConfig, ShipConfig *shipConfig){
            shipConfig->emitter = newEmitter;
            this->player->ship->onConfigDidUpdate();
        });
        items.push_back(item);
    }
    for (int i = 0; i < 3; i++) {
        CollectorConfig newCollector = CollectorConfig(CollectorBasic, 10 + i * 2);
        int price = 10 + i * 3;
        
        std::string title = string_format("Collector \"%s\"", collectorTypeString(newCollector.category).c_str());
        std::string description = string_format("Capacity: %.0f\nPrice: %d", newCollector.capacity, price);
        auto item = TradingItem(price, title, description, [newCollector, this](PlayerConfig *playerConfig, ShipConfig *shipConfig){
            shipConfig->collector = newCollector;
            this->player->ship->onConfigDidUpdate();
        });
        items.push_back(item);
    }
    TraderConfig traderConfig{100, items};
    trader = new Trader(b2Vec2(-80, -80), traderConfig, world);
    trader->building->onPlayerEnter = [this]{
        this->activeTrader = trader;
    };
    trader->building->onPlayerLeave = [this]{
        this->activeTrader = nullptr;
    };
}

void GameScene::step(float dt)
{
    if (isPaused)
        return;
    
    camera.x = player->ship->getPosition().x;
    camera.y = player->ship->getPosition().y;
    
    auto movement = normalize(playerMovement);
    player->ship->move(movement);
    player->ship->rotate(playerRotation);
    player->ship->step(dt);
    
    world->step(dt);
}

void GameScene::render()
{
    world->render(window, camera);
    player->render(window, camera);
}

void GameScene::handleKeyDown(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::D) {
        playerMovement.x += 1;
    }
    if (key == sf::Keyboard::A) {
        playerMovement.x -= 1;
    }
    
    if (key == sf::Keyboard::W) {
        playerMovement.y -= 1;
    }
    if (key == sf::Keyboard::S) {
        playerMovement.y += 1;
    }
    
    if (key == sf::Keyboard::Tab && onShowInventory) {
        playerMovement = b2Vec2_zero;
        onShowInventory(player);
    }
    
    if (key == sf::Keyboard::Space && activeTrader && onShowTrader) {
        playerMovement = b2Vec2_zero;
        onShowTrader(player, activeTrader);
    }
}

void GameScene::handleKeyUp(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::D) {
        playerMovement.x -= 1;
    }
    if (key == sf::Keyboard::A) {
        playerMovement.x += 1;
    }
    
    if (key == sf::Keyboard::W) {
        playerMovement.y += 1;
    }
    if (key == sf::Keyboard::S) {
        playerMovement.y -= 1;
    }
}

void GameScene::handleMouseMove(sf::Vector2i screenPos)
{
    float px = window->getSize().x / 2;
    float py = window->getSize().y / 2;
    float angle = atan2(screenPos.y - py, screenPos.x - px);
    playerRotation = angle;
}

void GameScene::handleMouseDown(sf::Vector2i screenPos)
{
    
}

void GameScene::handleMouseUp(sf::Vector2i screenPos)
{
    
}
