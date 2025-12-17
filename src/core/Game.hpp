#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "../game/screens/ScreenManager.hpp"

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
    ResourceManager resourceManager;
    ScreenManager screenManager;
    sf::Clock clock;
    double delta;
private:
    void handleInput(const std::optional<sf::Event> &event) const;
    void update();
    void draw();
};
