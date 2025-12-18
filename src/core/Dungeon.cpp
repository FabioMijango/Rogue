#include "Dungeon.hpp"

dg::Room::Room() {
    position = {0, 0};
    size = {0, 0};
}

dg::Room::Room(const sf::Vector2f position, const sf::Vector2u size) {
    this->position = position;
    this->size = size;
}

sf::FloatRect dg::Room::getBounds() const {
    return {sf::Vector2<float>(position), sf::Vector2<float>(size)};
}


dg::Dungeon::Dungeon() {
    rooms = std::vector<Room>();
    edges = std::vector<Edge>();
}

dg::Dungeon::Dungeon(std::vector<Room> rooms, std::vector<Edge> edges, std::unordered_map<uint16_t, sf::Sprite>& tileCache) {
    this->rooms = std::move(rooms);
    this->edges = std::move(edges);
    this->tileCache = std::move(tileCache);
}
