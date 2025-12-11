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

dg::Dungeon::Dungeon(std::vector<Room> rooms, std::vector<Edge> edges) {
    this->rooms = std::move(rooms);
    this->edges = std::move(edges);
}

void dg::Dungeon::print() {
    for (const auto& room : rooms) {
        std::cout << "Room at (" << room.position.x << ", " << room.position.y << ") size (" << room.size.x << ", " << room.size.y << ")" << std::endl;
    }

    //print edges
    for (const auto& edge : edges) {
        std::cout << "Edge from Room " << edge.a << " to Room " << edge.b << std::endl;
    }
}
