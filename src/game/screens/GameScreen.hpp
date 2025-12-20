#pragma once
#include <SFML/Graphics/Text.hpp>

#include "Screen.hpp"
#include "../../core/Dungeon.hpp"
#include "../../core/DungeonGenerator.hpp"

class GameScreen : public Screen {
public:
    void handleInput(const std::optional<sf::Event> &event) override;
    void update(double dt) override;
    void draw() override;
    void onEnter(GameContext context) override;
    void onExit() override;

private:
    dg::Dungeon dungeon;

};
