#include "EntityManager.hpp"

void EntityManager::init(sf::Sprite playerSprite, std::vector<dg::Room> *rooms) {
    this->player = std::make_unique<Player>(playerSprite);
    player->position = {4.5f * 32.0f, 4.5f * 32.0f};
    this->rooms = rooms;
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
