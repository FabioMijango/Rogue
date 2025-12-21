#include "Player.hpp"

#include "states/PlayerNormal.hpp"

Player::Player(sf::Sprite sprite): Entity(sprite) {
    this->position = {0.0f, 0.0f};
    this->currentState = std::make_unique<PlayerNormal>(position);
}

void Player::handleInput(const std::optional<sf::Event> &event) {
    const auto newState = dynamic_cast<PlayerState*>(this->currentState.get())->handleInput(event);
    if (newState) {
        //TODO
    }
}

void Player::update(double dt) {
    const auto newState = this->currentState->update(dt);
    sprite.setPosition(position);
    if (newState) {
        //TODO
    }
}

void Player::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
    sf::View v = window.getView();
    v.setCenter(position);
    window.setView(v);
}
