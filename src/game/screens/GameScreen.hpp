#pragma once
#include <SFML/Graphics/Text.hpp>

#include "Screen.hpp"

class GameScreen : public Screen {
public:
    void handleInput(const std::optional<sf::Event> &event) override;
    void update(double dt) override;
    void render() override;
    void onEnter(GameContext context) override;
    void onExit() override;

};
