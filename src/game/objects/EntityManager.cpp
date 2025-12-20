#include "EntityManager.hpp"

void EntityManager::init(sf::Sprite playerSprite)
{
    this->player = std::make_unique<Player>(playerSprite);
}

void EntityManager::handleInput(const std::optional<sf::Event> &event) {
    player->handleInput(event);
}

void EntityManager::update(double dt) {
    player->update(dt);
}

void EntityManager::draw(sf::RenderWindow &window) const {
    player->draw(window);
}
