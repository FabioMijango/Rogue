#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "../game/screenManager/ScreenManager.hpp"
#include "../game/screenManager/StartScreen.hpp"

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
