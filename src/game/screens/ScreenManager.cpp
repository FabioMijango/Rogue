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

void ScreenManager::update(double dt) {
    currentScreen->update(dt);

    ScreenId pendingScreen = currentScreen->getPendingScreen();
    if (pendingScreen != ScreenId::None)
        changeScreen(getScreenById(pendingScreen));
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

std::unique_ptr<Screen> ScreenManager::getScreenById(const ScreenId id) {
    std::unique_ptr<Screen> result;
    switch (id) {
        case ScreenId::Game:
            result = std::make_unique<GameScreen>();
            break;
        case ScreenId::Start:
            result = std::make_unique<StartScreen>();
            break;
        default:
            throw std::runtime_error("Invalid screen id");
    }
    return result;
}
