#pragma once
#include "SFML/Graphics.hpp"

class BaseScene
{
protected:
    sf::RenderWindow *window;
    sf::Clock deltaClock;
    
public:
    BaseScene(sf::RenderWindow *window);
    
    void handleEvent(sf::Event event);
    
    virtual void handleKeyDown(sf::Keyboard::Key key) { }
    virtual void handleKeyUp(sf::Keyboard::Key key) { }
    virtual void handleMouseMove(sf::Vector2i screenPos) { }
    virtual void handleMouseDown(sf::Vector2i screenPos) { }
    virtual void handleMouseUp(sf::Vector2i screenPos) { }
    
    virtual void step(float dt) = 0;
    virtual void render() = 0;
};
