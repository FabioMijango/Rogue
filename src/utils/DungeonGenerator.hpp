#pragma once
#include <algorithm>
#include <cstdint>
#include <random>
#include <ranges>
#include <unordered_map>

#include "SGEL.hpp"
#include "../Bible.hpp"

struct Position {
    int x = 0, y = 0;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct Connection {
    Position cellA, cellB;

    bool operator==(const Connection& other) const {
        return cellA == other.cellA && cellB == other.cellB;
    }
};

struct PairRoom {
    uint32_t roomA;
    uint32_t roomB;

    bool operator==(const PairRoom& other) const {
        return roomA == other.roomA && roomB == other.roomB;
    }
};


inline void hash_combine(size_t& seed, size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
    template <>
    struct hash<Position> {
        size_t operator()(const Position& pos) const noexcept {
            size_t seed = 0;
            hash_combine(seed, hash<int>()(pos.x));
            hash_combine(seed, hash<int>()(pos.y));
            return seed;
        }
    };

    template <>
    struct hash<Connection> {
        size_t operator()(const Connection& conn) const noexcept {
            size_t seed = 0;
            hash_combine(seed, hash<Position>()(conn.cellA));
            hash_combine(seed, hash<Position>()(conn.cellB));
            return seed;
        }
    };

    template <>
    struct hash<PairRoom> {
        size_t operator()(const PairRoom& p) const {
            size_t seed = 0;
            hash_combine(seed, hash<uint32_t>()(p.roomA));
            hash_combine(seed, hash<uint32_t>()(p.roomB));
            return seed;
        }
    };
}

enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// =====================================
enum class TileType : uint8_t {
    Floor,
    Wall,
};

struct CellTile {
    SDL_FRect gridPos;        // Posición lógica en la cuadrícula (x, y)
    TileType type;           // Tipo de celda
    uint32_t roomId;         // A qué sala pertenece
    const Animation& animation;  // Referencia a la animación que representa esta celda
};

struct Room {
    uint32_t id;
    SDL_FRect worldBounds;            // Límites (AABB) de la sala en coordenadas de mundo

    std::vector<CellTile> cells;
};

struct Dungeon {
    float roomSize = 0;
    uint32_t tilesPerRoom = 0;
    // std::unordered_map<Position, CellTile> tiles = {};
    std::unordered_map<uint32_t, Room> m_rooms = {};
};

class DungeonGenerator {
    std::mt19937 gen;
    std::uniform_int_distribution<> dirDistribution;
    std::uniform_real_distribution<float> diceDistribution;

    std::unordered_map<Position, uint32_t> m_rooms;
    std::vector<Connection> m_connections;

    Dungeon dungeon;

    static Position newPos(Direction dir, const Position &from);

    void prepare(float roomSize, uint32_t tilesPerRoom);
    void generateRooms(uint32_t roomsNum);
    void connectRooms();
    void populateRooms();
    void resolveRoomsConnections();
    void generateCollisions(EntityManager& entityManager);

    std::vector<CellTile> populateRoomTiles(Position roomPos, uint32_t roomsNum) const;

public:
    DungeonGenerator();

    Dungeon generate(uint32_t roomsNum, float roomSize, uint32_t tilesPerRoom);

    auto& getRooms() {
        return m_rooms;
    }

    auto& getConnections() {
        return m_connections;
    }

};
