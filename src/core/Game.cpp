#include "Game.hpp"

Game::Game() {
    delta = clock.restart().asSeconds();
}

void Game::update() {
    return;
}

void Game::draw() {
    return;
}


void Game::run() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Rogue");
    window.setFramerateLimit(60);

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
        window.clear();
        draw();
        window.display();
    }
}
