#include "Player.hpp"

#include <utility>

Player::Player(sf::Sprite sprite): Entity(sprite) {
    this->position = {0.0f, 0.0f};
}

void Player::handleInput(const std::optional<sf::Event> &event) {
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::W) {
            position.y += -100.0f;
        } else if (keyPressed->scancode == sf::Keyboard::Scan::S) {
            position.y += 100.0f;
        }
        if (keyPressed->scancode == sf::Keyboard::Scan::A) {
            position.x += -100.0f;
        } else if (keyPressed->scancode == sf::Keyboard::Scan::D) {
            position.x += 100.0f;
        }
    }
}

void Player::update(double dt) {
    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}
