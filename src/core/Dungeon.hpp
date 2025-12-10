#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace dg {
    struct Edge {
        int a, b;
    };

    struct Room {
        sf::Vector2<int> position;
        sf::Vector2<unsigned int> size;
        std::vector<std::vector<uint16_t>> tileMap;
        Room();
        Room(sf::Vector2<int> position, sf::Vector2<unsigned int> size);
        [[nodiscard]] sf::FloatRect getBounds() const;
    };

    class Dungeon {
    public:
    public:
        Dungeon();
        Dungeon(std::vector<Room> rooms, std::vector<Edge> edges);

        void print();
    private:
        std::vector<Room> rooms;
        std::vector<Edge> edges;
    };
}
