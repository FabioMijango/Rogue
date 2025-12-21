#include "PlayerNormal.hpp"

PlayerNormal::PlayerNormal(sf::Vector2f &position): PlayerState(position) {
}

std::unique_ptr<State> PlayerNormal::handleInput(const std::optional<sf::Event> &event) {
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

    return nullptr;
}

std::unique_ptr<State> PlayerNormal::update(double dt) {
    position += currentVelocity * static_cast<float>(dt);
    return nullptr;
}

void PlayerNormal::render() {
}

void PlayerNormal::onEnter() {
}

void PlayerNormal::onExit() {
}
