#include "game_scene.hpp"
#include "constants.h"
#include "string_utils.hpp"

#include "solid_block.hpp"

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
    camera.scale = 30.0f;
    
    world = new PhysicsWorld();
    
    PlayerConfig playerConfig{20.0f, 20.0f, 350};
    FrameConfig frameConfig{FrameBasic, 0.0f, 10.0f};
    EmitterConfig emitterConfig{EmitterBasic, 50.0f, 500.0f, 0.2f};
    CollectorConfig collectorConfig{CollectorBasic, 10.0f};
    WeaponConfig weaponConfig(10.0f, 2);
    ShipConfig shipConfig{frameConfig, emitterConfig, collectorConfig, weaponConfig, 1900.0f, 3000.0f};
    player = new Player(b2Vec2(-100, 0), playerConfig, shipConfig, world);
    
    mainPlanet = new Planet(b2Vec2(-20, -40), 5, 30, world);
    mainAsteroids = new AsteroidBelt(mainPlanet->center, mainPlanet->gravityRadius, mainPlanet->gravityRadius + 5, 1, world);
    
//    int count = sizeof(allMaterialTypes) / sizeof(MaterialType);
//    for (int i = 0; i < count; i++) {
//        auto mt = allMaterialTypes[i];
//        int x = -400 + i * 30;
//        float radius = 12.0f;
//        auto bp = PolygonUtils::Polygon::makeCircle(b2Vec2(x, -30), radius, 8, mt, true, true);
//        auto block = new SolidBlock(bp, false, world);
//        world->addEntity(block);
//    }
    
    int count = sizeof(allMaterialTypes) / sizeof(MaterialType);
    for (int i = 0; i < count; i++) {
        auto mt = allMaterialTypes[i];
        int x = -400 + i * 30;
        float radius = 5 + i * 2;
        auto bp = PolygonUtils::Polygon::makeCircle(b2Vec2(x, -30), radius, 4, mt, true, true);
        auto block = new SolidBlock(bp, false, world);
        world->addEntity(block);
    }
    
    auto planet2 = new Planet(b2Vec2(60, 5), 30, 40, world);
    auto asteroids2 = new AsteroidBelt(planet2->center, planet2->gravityRadius, planet2->gravityRadius + 10, 0.8, world);

    auto planet3 = new Planet(b2Vec2(-30, 40), 15, 35, world);
    auto asteroids3 = new AsteroidBelt(planet3->center, planet3->gravityRadius, planet3->gravityRadius + 4, 0.5, world);
    
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
        std::string description = string_format("Radius: %.0f\nPrice: %d", newCollector.orbCollectionRadius, price);
        auto item = TradingItem(price, title, description, [newCollector, this](PlayerConfig *playerConfig, ShipConfig *shipConfig){
            shipConfig->collector = newCollector;
            this->player->ship->onConfigDidUpdate();
        });
        items.push_back(item);
    }
    
    TraderConfig traderConfig{100, items};
    trader = new Trader(b2Vec2(-8, -8), traderConfig, world);
    trader->building->onPlayerEnter = [this]{
        this->activeTrader = trader;
    };
    trader->building->onPlayerLeave = [this]{
        this->activeTrader = nullptr;
    };
    
    FuelStationConfig fuelStationConfig{11240.0f, 1.2f};
    fuelStation = new FuelStation(b2Vec2(8, 8), fuelStationConfig, world);
    fuelStation->building->onPlayerEnter = [this]{
        this->activeFuelStation = fuelStation;
    };
    fuelStation->building->onPlayerLeave = [this]{
        this->activeFuelStation = nullptr;
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
    
    float viewportWidth = window->getSize().x / camera.scale * DEBUG_RENDER_VIEWPORT;
    float viewportHeight = window->getSize().y / camera.scale * DEBUG_RENDER_VIEWPORT;
    world->updateChunks(b2Vec2(camera.x, camera.y), viewportWidth, viewportHeight);
    
    world->step(dt);
}

void GameScene::render()
{
    float viewportWidth = window->getSize().x * DEBUG_RENDER_VIEWPORT;
    float viewportHeight = window->getSize().y * DEBUG_RENDER_VIEWPORT;
    
    sf::RectangleShape chunkRect;
    chunkRect.setSize(sf::Vector2f{viewportWidth, viewportHeight});
    chunkRect.setPosition((window->getSize().x - viewportWidth) / 2, (window->getSize().y - viewportHeight) / 2);
    chunkRect.setFillColor(sf::Color::Transparent);
    chunkRect.setOutlineColor(sf::Color::Green);
    chunkRect.setOutlineThickness(2.0f);
    window->draw(chunkRect);
    
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
    
    if (key == sf::Keyboard::Space) {
        if (activeTrader && onShowTrader) {
            playerMovement = b2Vec2_zero;
            onShowTrader(player, activeTrader);
        } else if (activeFuelStation && onShowFuelStation) {
            playerMovement = b2Vec2_zero;
            onShowFuelStation(player, activeFuelStation);
        }
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
    if (!player || !player->ship)
        return;
    player->ship->startFire();
}

void GameScene::handleMouseUp(sf::Vector2i screenPos)
{
    if (!player || !player->ship)
        return;
    player->ship->stopFire();
}
