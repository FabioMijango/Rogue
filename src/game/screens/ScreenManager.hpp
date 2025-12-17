#pragma once
#include "GameScreen.hpp"
#include "Screen.hpp"
#include "StartScreen.hpp"
#include "../GameContext.hpp"
#include "../../core/ResourceManager.hpp"

class ScreenManager {
public:
    ScreenManager() = default;
    void init(GameContext ctx);
    void handleInput(const std::optional<sf::Event> &event) const;
    void update(double dt);
    void render() const;

private:
    GameContext context;
    std::unique_ptr<Screen> currentScreen;
    std::unique_ptr<Screen> previousScreen;
private:
    void changeScreen(std::unique_ptr<Screen> newScreen);
    [[nodiscard]] std::unique_ptr<Screen> getScreenById(ScreenId id) ;
};
