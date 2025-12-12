#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "../game/screens/ScreenManager.hpp"
#include "../game/screens/StartScreen.hpp"

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
    void handleInput();
    void update();
    void draw();
};
