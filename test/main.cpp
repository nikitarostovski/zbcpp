#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "constants.h"

int main()
{
    const uint32_t window_width  = DEBUG_RESOLUTION_X;
    const uint32_t window_height = DEBUG_RESOLUTION_Y;
    const uint32_t fps = 60;
    
    sf::View viewport(sf::FloatRect(0.f, 0.f, window_width, window_height));
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Test", sf::Style::Close, settings);
    window.setFramerateLimit(fps);
    window.setView(viewport);
    window.setKeyRepeatEnabled(false);
    
    Game *game = new Game(&window);
    game->onQuit = [&window]() -> void {
        window.close();
    };
    
    auto clock = sf::Clock();
    
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    game->handleEvent(event);
                    break;
            }
        }
        float dt = 1.0/fps;
        game->step(dt);
        
        auto t = clock.getElapsedTime();
        printf("%.dms\n", t.asMilliseconds());
        clock.restart();
    }
    return 0;
}
