#include "ScreenManager.hpp"

void ScreenManager::init() {
    currentScreen = nullptr;
    previousScreen = nullptr;
}

void ScreenManager::handleInput(const std::optional<sf::Event> &event) const {
    currentScreen->handleInput(event);
}

void ScreenManager::update(double dt) const {
    currentScreen->update(dt);
}

void ScreenManager::render() const {
    currentScreen->render();
}

void ScreenManager::changeScreen(std::unique_ptr<Screen> newScreen) {
    previousScreen = std::move(currentScreen);
    currentScreen = std::move(newScreen);
}
