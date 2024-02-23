#include "player.hpp"
#include "constants.h"
#include "resource_path.hpp"

Player::Player(b2Vec2 pos, PlayerConfig playerConfig, ShipConfig shipConfig, PhysicsWorld *world)
{
    config = playerConfig;
    
    mainFont = sf::Font();
    mainFont.loadFromFile(resourcePath() + "Signwood-Regular.ttf");
    
    materialText = sf::Text();
    materialText.setFont(mainFont);
    materialText.setCharacterSize(14);
    materialText.setFillColor(sf::Color::White);
    materialText.setOutlineColor(sf::Color::Blue);
    materialText.setOutlineThickness(1);
    
    
    healthBar = sf::RectangleShape();
    healthBar.setFillColor(sf::Color::Red);
    healthBarBackground = sf::RectangleShape();
    healthBarBackground.setFillColor(sf::Color(127, 127, 127, 127));
    
    fuelBar = sf::RectangleShape();
    fuelBar.setFillColor(sf::Color::Blue);
    fuelBarBackground = sf::RectangleShape();
    fuelBarBackground.setFillColor(sf::Color(127, 127, 127, 127));
    
    ship = new Ship(pos, shipConfig, world);
    ship->onDamageReceive = [this](float damage){
        int health = this->config.health;
        health = std::max(0.0f, (health - damage));
        this->config.health = health;
        if (health == 0) {
            printf("GAME OVER\n");
        }
    };
    ship->onMaterialCollect = [this](int amount){
        this->config.material += amount;
    };
    world->addEntity(ship);
}

Player::~Player()
{
    delete ship;
}

void Player::render(sf::RenderWindow *window, Camera camera)
{
    float x = 64;
    float y = 64;
    
    std::string materials = std::to_string(config.material);

    materialText.setPosition(window->getSize().x - 100, y);
    materialText.setString(materials);
    materialText.setOrigin(materialText.getGlobalBounds().width / 2,
                           materialText.getGlobalBounds().height / 2);
    window->draw(materialText);
    
    
    const float healthBarHeight = 20.0f;
    const float healthBarWidth = 200.0f;
    
    float healthBarActualWidth = healthBarWidth * config.health / config.maxHealth;
    
    healthBarBackground.setSize(sf::Vector2f(healthBarWidth, healthBarHeight));
    healthBar.setSize(sf::Vector2f(healthBarActualWidth, healthBarHeight));
    healthBarBackground.setPosition(x, y);
    healthBar.setPosition(x, y);
    
    window->draw(healthBarBackground);
    window->draw(healthBar);
    
    float fuelBarActualWidth = healthBarWidth * ship->shipConfig.fuel / ship->shipConfig.maxFuel;
    
    fuelBarBackground.setSize(sf::Vector2f(healthBarWidth, healthBarHeight));
    fuelBar.setSize(sf::Vector2f(fuelBarActualWidth, healthBarHeight));
    fuelBarBackground.setPosition(x, y + 40);
    fuelBar.setPosition(x, y + 40);
    
    window->draw(fuelBarBackground);
    window->draw(fuelBar);
}
