#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "../core/ResourceManager.hpp"

struct GameContext {
    ResourceManager *resourceManager;
    sf::RenderWindow *window;
};
