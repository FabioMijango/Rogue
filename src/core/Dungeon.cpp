#include "Dungeon.hpp"

dg::Room::Room() {
    position = {0, 0};
    size = {0, 0};
}

dg::Room::Room(const sf::Vector2<int> position, const sf::Vector2<unsigned int> size) {
    this->position = position;
    this->size = size;
}

sf::FloatRect dg::Room::getBounds() const {
    return {sf::Vector2<float>(position), sf::Vector2<float>(size)};
}


dg::Dungeon::Dungeon() {
}
