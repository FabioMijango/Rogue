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

void dg::Room::draw(sf::RenderWindow &window, const std::unordered_map<uint16_t, sf::Sprite> &tileCache) const {
    for (uint16_t tileX = 0; tileX < size.x; tileX++) {
        for (uint16_t tileY = 0; tileY < size.y; tileY++) {
            auto tileId = tileMap[tileX][tileY];
            sf::Sprite sprite = tileCache.at(tileId);
            sprite.setPosition(sf::Vector2f{ (position.x + tileX) * 32.f, (position.y + tileY) * 32.f });
            window.draw(sprite);
        }
    }
}

dg::Dungeon::Dungeon() {
    rooms = std::vector<Room>();
    edges = std::vector<Edge>();
}

dg::Dungeon::Dungeon(
    std::vector<Room> rooms,
    std::vector<Edge> edges,
    std::unordered_map<uint16_t, sf::Sprite>& tileCache,
    EntityManager entityManager) {
    this->rooms = std::move(rooms);
    this->edges = std::move(edges);
    this->tileCache = std::move(tileCache);
    this->entityManager = std::move(entityManager);
}

void dg::Dungeon::draw(sf::RenderWindow &window) const {
    for (auto& room : rooms) {
        room.draw(window, tileCache);
    }
    entityManager.draw(window);
}
