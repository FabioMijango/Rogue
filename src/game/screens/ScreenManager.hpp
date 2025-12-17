#pragma once
#include "../../core/ResourceManager.hpp"
#include "Screen.hpp"

class ScreenManager {
public:
    ScreenManager() = default;
    void init();
    void handleInput(const std::optional<sf::Event> &event) const;
    void update(double dt) const;
    void render() const;
    void changeScreen(std::unique_ptr<Screen> newScreen);

private:
    ResourceManager *resourceManager;
    std::unique_ptr<Screen> currentScreen;
    std::unique_ptr<Screen> previousScreen;
};
