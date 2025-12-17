#include "GameScreen.hpp"

void GameScreen::handleInput(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::Closed>())
        window->close();

    else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {

    }
}

void GameScreen::update(double dt) {

}

void GameScreen::render() {
    for (const auto& drawable : drawables) {
        window->draw(*drawable);
    }
}

void GameScreen::onEnter(const GameContext context) {
    window = context.window;
    const std::weak_ptr<sf::Font> font = context.resourceManager->getFont(bb::fontResources[0].name);

    sf::Text titleText(*font.lock(), "GAME SCREEN", 48);
    titleText.setPosition({0, 0});
    drawables.push_back(std::make_shared<sf::Text>(titleText));
}

void GameScreen::onExit() {
    drawables.clear();
}
