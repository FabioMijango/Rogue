#include "ScreenManager.hpp"

void ScreenManager::init(const GameContext ctx) {
    context = ctx;
    currentScreen = nullptr;
    previousScreen = nullptr;

    StartScreen startScreen;
    startScreen.onEnter(context);
    changeScreen(std::make_unique<StartScreen>(startScreen));
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

    currentScreen->onEnter(context);
    if (previousScreen) {
        previousScreen->onExit();
    }
}
