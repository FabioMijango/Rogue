#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace dg {
    struct Edge {
        int a, b;
    };

    struct Room {
        sf::Vector2f position;
        sf::Vector2u size;
        std::vector<std::vector<uint16_t>> tileMap;
        Room();
        Room(sf::Vector2f position, sf::Vector2u size);
        [[nodiscard]] sf::FloatRect getBounds() const;
    };

    class Dungeon {
    public:
        Dungeon();
        Dungeon(std::vector<Room> rooms, std::vector<Edge> edges, std::unordered_map<uint16_t, sf::Sprite>& tileCache);

    private:
        std::vector<Room> rooms;
        std::vector<Edge> edges;
        std::unordered_map<uint16_t, sf::Sprite> tileCache;
    };
}
