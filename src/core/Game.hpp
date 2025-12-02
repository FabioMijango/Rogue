#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
    ResourceManager resourceManager;
    sf::Clock clock;
    double delta;
private:
    void update();
    void draw();
};
