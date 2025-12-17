#pragma once
#include "Screen.hpp"
#include "StartScreen.hpp"
#include "../GameContext.hpp"
#include "../../core/ResourceManager.hpp"

class ScreenManager {
public:
    ScreenManager() = default;
    void init(GameContext ctx);
    void handleInput(const std::optional<sf::Event> &event) const;
    void update(double dt) const;
    void render() const;
    void changeScreen(std::unique_ptr<Screen> newScreen);

private:
    GameContext context;
    std::unique_ptr<Screen> currentScreen;
    std::unique_ptr<Screen> previousScreen;
};
