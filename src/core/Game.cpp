#include "Game.hpp"

Game::Game() {
    window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Rogue");
    window.setFramerateLimit(60);

    const sf::View view({0.f, 0.f}, sf::Vector2f(window.getSize()));
    window.setView(view);

    screenManager.init();
    StartScreen startScreen;
    startScreen.onEnter(resourceManager, window);
    screenManager.changeScreen(std::make_unique<StartScreen>(startScreen));

    delta = clock.restart().asSeconds();
}

void Game::handleInput(const std::optional<sf::Event> &event) const {
    screenManager.handleInput(event);
}

void Game::update() {
    delta = clock.restart().asSeconds();
    screenManager.update(delta);
}

void Game::draw() {
    window.clear();
    screenManager.render();
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            handleInput(event);
        }
        update();
        draw();
    }
}
