#include "Game.hpp"

Game::Game() {
    window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Rogue");
    window.setFramerateLimit(60);

    delta = clock.restart().asSeconds();
}

void Game::update() {
    return;
}

void Game::draw() {
    window.clear();

    window.display();
}


void Game::run() {

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        delta = clock.restart().asSeconds();

        update();

        draw();
    }
}
