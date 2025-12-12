#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Screen.hpp"

class StartScreen : public Screen {
public:
    void update(double dt) override;
    void render() override;
    void handleInput() override;
    void onEnter(ResourceManager &resourceManager, sf::RenderWindow &window_) override;
    void onExit() override;
};
