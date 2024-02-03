#pragma once
#include <SFML/Graphics.hpp>

enum ButtonState {
    Normal,
    Hover,
    Pressed
};

class Button
{
private:
    sf::RectangleShape background;
    sf::Text title;
    
    sf::Rect<float> frame;
    ButtonState state;
    
    sf::Color normalBackgroundColor;
    sf::Color hoverBackgroundColor;
    sf::Color pressedBackgroundColor;
    
    void setState(ButtonState newState);
    void updateState();
public:
    std::string titleText;
    std::function<void()> callback;
    
    Button(std::string title, sf::Rect<float> frame);
    
    void setFrame(sf::Rect<float> frame);
    sf::Rect<float> getFrame();
    
    void render(sf::RenderWindow *window);
    
    void handleMouseMove(sf::Vector2f pos);
    void handleMouseDown(sf::Vector2f pos);
    void handleMouseUp(sf::Vector2f pos);
};
