#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "../GameContext.hpp"
#include "../../core/ResourceManager.hpp"

class Screen {
public:
    virtual ~Screen() = default;
    virtual void handleInput(const std::optional<sf::Event> &event) = 0;
    virtual void update(double dt) = 0;
    virtual void render() = 0;
    virtual void onEnter(GameContext context) = 0;
    virtual void onExit() = 0;

protected:
    sf::RenderWindow *window = nullptr;
    std::vector<std::shared_ptr<sf::Drawable>> drawables;
};
