#include "StartScreen.hpp"

void StartScreen::update(double dt) {
    if (clock.getElapsedTime() > sf::seconds(1.f)) {
        colorChanged = !colorChanged;
        clock.restart();
        for (auto &drawable: drawables) {
            if (auto *text = dynamic_cast<sf::Text *>(drawable.get())) {
                text->setFillColor(colorChanged ? sf::Color(128, 128, 128) : sf::Color::White);
            }
        }
    }
}

void StartScreen::render() {
    for (const auto &drawable: drawables) {
        window->draw(*drawable);
    }
}

void StartScreen::handleInput(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::Closed>())
        window->close();

    if (event->is<sf::Event::KeyPressed>()) {
        if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
            window->close();
    }
}

void StartScreen::onEnter(ResourceManager &resourceManager, sf::RenderWindow &window_) {
    window = &window_;
    clock.restart();
    const std::weak_ptr<sf::Font> font = resourceManager.getFont(bb::fontResources[0].name);

    sf::Text titleText(*font.lock(), "ROGUE", 48);
    titleText.setPosition({0, 0});
    drawables.push_back(std::make_shared<sf::Text>(titleText));

    sf::Text descText(*font.lock(), "Press any button", 24);
    descText.setPosition({0, 60});
    drawables.push_back(std::make_shared<sf::Text>(descText));
}

void StartScreen::onExit() {
    drawables.clear();
}
