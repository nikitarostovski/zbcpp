#include "player.hpp"
#include "constants.h"
#include "resource_path.hpp"

Player::Player(b2Vec2 pos, PlayerConfig playerConfig, ShipConfig shipConfig, PhysicsWorld *world)
{
    config = playerConfig;
    
    mainFont = sf::Font();
    mainFont.loadFromFile(resourcePath() + "Signwood-Regular.ttf");
    
    healthText = sf::Text();
    healthText.setFont(mainFont);
    healthText.setCharacterSize(18);
    healthText.setFillColor(sf::Color::White);
    healthText.setOutlineColor(sf::Color::Red);
    healthText.setOutlineThickness(1);
    
    materialText = sf::Text();
    materialText.setFont(mainFont);
    materialText.setCharacterSize(14);
    materialText.setFillColor(sf::Color::White);
    materialText.setOutlineColor(sf::Color::Blue);
    materialText.setOutlineThickness(1);
    
    ship = new ControlledShip(pos, shipConfig, world);
    ship->frame->onDamageReceive = [this](float damage){
        this->config.health -= damage;
    };
    ship->collector->onMaterialCollect = [this](int amount){
        this->config.material += amount;
    };
}

Player::~Player()
{
    delete ship;
}

void Player::render(sf::RenderWindow *window, Camera camera)
{
    float x = (ship->getPosition().x - camera.x) * PPM + window->getSize().x / 2;
    float y = (ship->getPosition().y - camera.y) * PPM + window->getSize().y / 2;
    
    std::string health = std::to_string((int)config.health);
    std::string materials = std::to_string(config.material);
    
    healthText.setPosition(x, y - 6);
    healthText.setString(health);
    healthText.setOrigin(healthText.getGlobalBounds().width / 2,
                         healthText.getGlobalBounds().height / 2);
    window->draw(healthText);
    
    materialText.setPosition(x, y + 10);
    materialText.setString(materials);
    materialText.setOrigin(materialText.getGlobalBounds().width / 2,
                           materialText.getGlobalBounds().height / 2);
    window->draw(materialText);
}
