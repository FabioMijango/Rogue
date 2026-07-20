#include "DungeonGenerator.hpp"

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

void DungeonGenerator::prepare(const float roomSize, const uint32_t tilesPerRoom) {
    m_rooms.clear();
    m_connections.clear();
    dungeon = Dungeon(roomSize, tilesPerRoom);
}

void DungeonGenerator::generateRooms(const uint32_t roomsNum) {
    Position currentPos{0, 0};
    m_rooms.insert({{0, 0}, 1});

    for (uint32_t i = 0; i < roomsNum; i++) {
        if (diceDistribution(gen) < 0.40f) {
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

    // std::unordered_map<std::pair<uint32_t, uint32_t>, std::vector<Connection>> connections;
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
        SDL_FRect roomBounds = {pos.x * dungeon.roomSize, pos.y * dungeon.roomSize, dungeon.roomSize, dungeon.roomSize};
        Room newRoom = {roomNum, roomBounds, roomTiles};
        dungeon.m_rooms.emplace(i++, newRoom);
    }
}

std::vector<CellTile> DungeonGenerator::populateRoomTiles(Position roomPos, const uint32_t roomsNum) const {
    auto& assets = Assets::Instance();
    auto& floor = assets.getAnimation("floor");
    auto& wall = assets.getAnimation("wall");

    std::vector<CellTile> roomTiles;
    roomTiles.reserve(dungeon.tilesPerRoom * dungeon.tilesPerRoom);
    for (int i = 0; i < dungeon.tilesPerRoom; i++) {
        for ( int j = 0; j < dungeon.tilesPerRoom; j++) {
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
            if (i == 0 || j == 0 || i == dungeon.tilesPerRoom - 1 || j == dungeon.tilesPerRoom - 1) {
                tileType = TileType::Wall;
            }
            // Room id
            uint32_t roomId = roomsNum;
            // Animation
            const Animation& tileAnimation = (tileType == TileType::Floor) ? floor : wall;

            roomTiles.emplace_back(tilePos, tileType, roomId, tileAnimation);
        }
    }
    return roomTiles;
}

void DungeonGenerator::resolveRoomsConnections() {
}

void DungeonGenerator::generateCollisions(EntityManager& entityManager) {
}

Dungeon DungeonGenerator::generate(uint32_t roomsNum, float roomSize, uint32_t tilesPerRoom) {
    prepare(roomSize, tilesPerRoom);
    generateRooms(roomsNum);
    connectRooms();
    populateRooms();
    resolveRoomsConnections();
    // generateCollisions(TODO);

    return dungeon;
}