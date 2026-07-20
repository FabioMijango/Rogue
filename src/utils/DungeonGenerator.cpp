#include "DungeonGenerator.hpp"

#include <iostream>

DungeonGenerator::DungeonGenerator() {
    gen = std::mt19937(std::random_device{}());
    dirDistribution = std::uniform_int_distribution<>(0, 3);
    diceDistribution = std::uniform_real_distribution<float>(0, 1);
}

Position DungeonGenerator::newPos(Direction dir, const Position &from)  {
    switch (dir) {
        case Direction::LEFT: return Position{from.x - 1, from.y};
        case Direction::RIGHT: return Position{from.x + 1, from.y};
        case Direction::UP: return Position{from.x, from.y + 1};
        case Direction::DOWN: return Position{from.x, from.y - 1};
    }
    throw std::logic_error("Invalid direction");
}

void DungeonGenerator::prepare() {
    m_rooms.clear();
    m_connections.clear();
    dungeon = Dungeon();
}

void DungeonGenerator::generateRooms() {
    Position currentPos{0, 0};
    m_rooms.insert({{0, 0}, 0});

    for (uint32_t i = 0; i < bb::NUMBER_OF_ROOMS; i++) {
        if (diceDistribution(gen) < bb::PROB_TO_BACK_TO_START) {
            currentPos = Position{0, 0};
        }

        float probToContinue = 0.8f;
        Position tempPos;
        do {
            probToContinue *= 0.8f;
            bool candidateRoom = false;

            std::vector<Direction> dirs = {Direction::LEFT, Direction::RIGHT, Direction::UP, Direction::DOWN};
            std::ranges::shuffle(dirs, gen);
            for (auto& newDir : dirs) {

                tempPos = newPos(newDir, currentPos);
                if (!m_rooms.contains(tempPos)) {
                    currentPos = tempPos;
                    candidateRoom = true;
                    break;
                }
            }
            if (!candidateRoom) {
                currentPos = newPos(dirs.front(), currentPos);
                break;
            }

            m_rooms.insert({Position{currentPos.x, currentPos.y}, i + 1});
        } while (diceDistribution(gen) < probToContinue);
    }
}

void DungeonGenerator::connectRooms() {
    std::unordered_map<PairRoom, std::vector<Connection>> connections;

    for (auto& [pos, room] : m_rooms) {
        for (Direction dir : {Direction::UP, Direction::RIGHT}) {
            Position neighbourPos =  newPos(dir, pos);
            auto it = m_rooms.find(neighbourPos);
            if (it == m_rooms.end()) continue;

            uint32_t neighborRoom = it->second;
            if (neighborRoom == room) continue; // misma sala, no es puerta

            auto [minRoom, maxRoom] = std::minmax(room, neighborRoom);
            const PairRoom key = {minRoom, maxRoom};
            connections[key].push_back(Connection{pos, neighbourPos});
        }
    }

    for (auto &options: connections | std::views::values) {
        std::uniform_int_distribution<> dist(0, options.size() - 1);
        m_connections.push_back(options[dist(gen)]);
    }
}

void DungeonGenerator::populateRooms() {
    uint32_t i = 0;
    for (auto& [pos, roomNum] : m_rooms) {
        auto roomTiles = populateRoomTiles(pos, roomNum);
        SDL_FRect roomBounds = {pos.x * bb::ROOM_SIZE, pos.y * bb::ROOM_SIZE, bb::ROOM_SIZE, bb::ROOM_SIZE};
        Room newRoom = {roomNum,{pos}, roomBounds, roomTiles};
        dungeon.m_rooms.emplace(i++, newRoom);
    }
}

std::vector<CellTile> DungeonGenerator::populateRoomTiles(Position roomPos, const uint32_t roomsNum) const {
    auto& assets = Assets::Instance();
    auto& floor = assets.getAnimation("floor");
    auto& wall = assets.getAnimation("wall");

    std::vector<CellTile> roomTiles;
    roomTiles.reserve(bb::TILES_PER_ROOM * bb::TILES_PER_ROOM);
    for (int i = 0; i < bb::TILES_PER_ROOM; i++) {
        for ( int j = 0; j < bb::TILES_PER_ROOM; j++) {
            // Position (X, Y) in world coordinates
            SDL_FRect tilePos = {
                // TODO Remove ' + 400'
                roomPos.x * bb::ROOM_SIZE + i * bb::TILES_PER_ROOM + 400,
                roomPos.y * bb::ROOM_SIZE + j * bb::TILES_PER_ROOM + 400,
                bb::TILES_PER_ROOM,
                bb::TILES_PER_ROOM
            };
            // Cell type
            TileType tileType = TileType::Floor;
            if (i == 0 || j == 0 || i == bb::TILES_PER_ROOM - 1 || j == bb::TILES_PER_ROOM - 1) {
                tileType = TileType::Wall;
            }
            // Room id
            uint32_t roomId = roomsNum;
            // Animation
            // TODO: Change to accept both types of wall (Wall Top, Wall Side)
            //      For the momento only uses Wall Side
            const Animation *tileAnimation = (tileType == TileType::Floor) ? &floor : &wall;

            roomTiles.emplace_back(tilePos, tileType, roomId, tileAnimation);
        }
    }
    return roomTiles;
}

void DungeonGenerator::resolveRoomsConnections() {
    std::vector<Direction> dirs = {Direction::LEFT, Direction::RIGHT, Direction::UP, Direction::DOWN};

    for (auto& room : dungeon.m_rooms | std::views::values) {
        for (Direction dir : dirs) {
            Position neighbourPos = newPos(dir, room.position);
            auto it = m_rooms.find(neighbourPos);
            if (it == m_rooms.end()) continue;

            auto neighbourRoomId = it->second;
            auto roomId = m_rooms.find(room.position)->second;
            if (roomId > neighbourRoomId) {
                std::swap(roomId, neighbourRoomId);
            }
            Connection connection = {room.position, neighbourPos};

            bool connectionFound = false;
            for (auto& conn : m_connections) {
                if ((conn.cellA == connection.cellA && conn.cellB == connection.cellB) ||
                    (conn.cellA == connection.cellB && conn.cellB == connection.cellA)) {
                    connectionFound = true;
                    break;
                }
            }

            bool sameRoom = false;
            if (roomId == neighbourRoomId) {
                connectionFound = true;
                sameRoom = true;
            }

            if (!connectionFound) continue;

            createConnectionBetweenRoom(dir, room.cells, sameRoom);
        }
    }

    // correctionForInitialRoom();
}

void DungeonGenerator::createConnectionBetweenRoom(Direction dir, std::vector<CellTile> &roomTiles, bool sameRoom) {
    auto& floor = Assets::Instance().getAnimation("floor");
    const auto size = bb::TILES_PER_ROOM;
    int doorStart, doorEnd;

    if (!sameRoom) {
        doorStart = size / 2 - 2;
        doorEnd = size / 2 + 1;
    } else {
        doorStart = 1;
        doorEnd = size - 2;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if ((dir == Direction::UP && i == size - 1 && j >= doorStart && j <= doorEnd) ||
                (dir == Direction::DOWN && i == 0 && j >= doorStart && j <= doorEnd) ||
                (dir == Direction::LEFT && j == 0 && i >= doorStart && i <= doorEnd) ||
                (dir == Direction::RIGHT && j == size - 1 && i >= doorStart && i <= doorEnd)) {
                roomTiles[j * size + i].animation = &floor;
            }
        }
    }
}

void DungeonGenerator::correctionForInitialRoom() {
    Position initRoom = {0, 0};
    int size = bb::TILES_PER_ROOM;
    int doorStart = size / 2 - 2;
    int doorEnd = size / 2 + 1;
    auto& wall = Assets::Instance().getAnimation("wall");
    for (auto& room : dungeon.m_rooms | std::views::values) {
        if (room.position == initRoom) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if ((i == 0 || i == size - 1) && (j < doorStart || j > doorEnd)) {
                        room.cells[j * size + i].animation = &wall;
                    }
                    else if ((j == 0 || j == size - 1) && (i < doorStart || i > doorEnd)) {
                        room.cells[j * size + i].animation = &wall;
                    }
                }
            }
            break;
        }
    }
}

Dungeon DungeonGenerator::generate() {
    prepare();
    generateRooms();
    connectRooms();
    populateRooms();
    resolveRoomsConnections();

    for (auto& [pos, room] : m_rooms) {
        std::cout << room <<"(" << pos.x << ", " << pos.y << ")\n";
    }

    return dungeon;
}