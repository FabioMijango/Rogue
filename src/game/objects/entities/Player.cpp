#include "Player.hpp"

Player::Player(sf::Sprite sprite): Entity(sprite) {
    this->position = {0.0f, 0.0f};
}

void Player::handleInput(const std::optional<sf::Event> &event) {
    currentVelocity = {0.0f, 0.0f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
            currentVelocity.y += -velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
            currentVelocity.y += velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
            currentVelocity.x += -velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
            currentVelocity.x += velocity;
        }
}

void Player::update(double dt) {
    position += currentVelocity * static_cast<float>(dt);
    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
    sf::View v = window.getView();
    v.setCenter(position);
    window.setView(v);
}
