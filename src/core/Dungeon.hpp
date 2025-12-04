#pragma once
#include <vector>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace dg {
    struct Room {
        sf::Vector2<int> position;
        sf::Vector2<unsigned int> size;
        Room();
        Room(sf::Vector2<int> position, sf::Vector2<unsigned int> size);
        [[nodiscard]] sf::FloatRect getBounds() const;
    };

    class Dungeon {
    public:
        std::vector<Room> rooms;
    public:
        Dungeon();
    };
}
