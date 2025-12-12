#pragma once
#include "../../core/ResourceManager.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Screen {
public:
    virtual ~Screen() = default;
    virtual void handleInput() = 0;
    virtual void update(double dt) = 0;
    virtual void render() = 0;
    virtual void onEnter(ResourceManager& resourceManager, sf::RenderWindow &window_) = 0;
    virtual void onExit() = 0;

protected:
    sf::RenderWindow *window = nullptr;
    std::vector<std::shared_ptr<sf::Drawable>> drawables;
};
