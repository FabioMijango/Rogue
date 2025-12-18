#include "GameScreen.hpp"

void GameScreen::handleInput(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::Closed>())
        window->close();

    else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {

    }

    else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
            sf::View view = window->getView();
            view.zoom(1.0f - mouseWheelScrolled->delta * 0.1f);
            window->setView(view);
        }
    }
}

void GameScreen::update(double dt) {

}

void GameScreen::render() {
    for (const auto& drawable : drawables) {
        window->draw(*drawable);
    }
    dungeon.draw(*window);
}

void GameScreen::onEnter(const GameContext context) {
    window = context.window;
    const std::weak_ptr<sf::Font> font = context.resourceManager->getFont(bb::fontResources[0].name);

    sf::Text titleText(*font.lock(), "GAME SCREEN", 48);
    titleText.setPosition({0, 0});
    drawables.push_back(std::make_shared<sf::Text>(titleText));

    dg::DungeonGenerator generator;
    dungeon = generator.generateDungeon(*context.resourceManager);
}

void GameScreen::onExit() {
    drawables.clear();
}
