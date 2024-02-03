#include "Button.hpp"

Button::Button(std::string title, sf::Rect<float> frame)
{
    this->titleText = title;
    
    sf::RectangleShape background;
    this->background = background;
    
    setFrame(frame);
    updateState();
}

void Button::setFrame(sf::Rect<float> frame)
{
    this->frame = frame;
    background.setPosition(frame.left, frame.top);
    background.setSize(frame.getSize());
}

sf::Rect<float> Button::getFrame()
{
    return frame;
}

void Button::handleMouseMove(sf::Vector2f pos)
{
    if (frame.contains(pos.x, pos.y)) {
        setState(ButtonState::Hover);
    } else {
        setState(ButtonState::Normal);
    }
}

void Button::handleMouseDown(sf::Vector2f pos)
{
    if (frame.contains(pos.x, pos.y)) {
        setState(ButtonState::Pressed);
    }
}

void Button::handleMouseUp(sf::Vector2f pos)
{
    if (state != ButtonState::Pressed)
        return;
    
    setState(ButtonState::Hover);
    
    if (frame.contains(pos.x, pos.y)) {
        callback();
    }
}

void Button::setState(ButtonState newState)
{
    if (state == newState)
        return;
    
    state = newState;
    updateState();
}

void Button::updateState()
{
    switch (state) {
        case Normal:
            background.setFillColor(sf::Color(64, 64, 64, 255));
            break;
        case Hover:
            background.setFillColor(sf::Color(127, 127, 127, 255));
            break;
        case Pressed:
            background.setFillColor(sf::Color(200, 200, 200, 255));
            break;
    }
}

void Button::render(sf::RenderWindow *window)
{
    window->draw(background);
}
