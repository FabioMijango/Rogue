#include "DungeonGenerator.hpp"

dg::Dungeon dg::DungeonGenerator::generateDungeon() {
    prepare();
    createRooms();
    fixRooms();
    connectRooms();
    return createDungeonFromData();
}

void dg::DungeonGenerator::prepare() {
    rooms.clear();
    roomCount = rnd::GetInt() % 3 + 10;
}

void dg::DungeonGenerator::createRooms() {
    Room initialRoom({0,0}, {10,10});
    rooms.push_back(initialRoom);

    for (size_t i = 0; i <= roomCount; i++) {
        Room newRoom(
            sf::Vector2<int>(rnd::GetInt() % 50, rnd::GetInt() % 50),
            sf::Vector2<unsigned int>(rnd::Get64() % 15 + 10, rnd::Get64() % 15 + 10)
            );
        rooms.push_back(newRoom);
    }
}

void dg::DungeonGenerator::fixRooms() {
    std::vector<Room> fixedRooms;
    for (const auto &room : rooms) {
        bool hasIntersection = false;
        for (const auto &fixedRoom : fixedRooms) {
            if (room.getBounds().findIntersection(fixedRoom.getBounds())) {
                std::cout << "Intersection" << std::endl;
                hasIntersection = true;
                break;
            }
        }
        if (!hasIntersection) {
            fixedRooms.push_back(room);
        }
    }
    rooms = std::move(fixedRooms);
}

void dg::DungeonGenerator::connectRooms() {
    edges.clear();
    std::vector<bool> connected(rooms.size(), false);
    connected[0] = true;

    int connectedCount = 1;
    const size_t roomSize = rooms.size();

    while (connectedCount < roomSize) {
        int bestA = -1, bestB = -1;
        int bestDist = INT_MAX;

        for (int i = 0; i < roomSize; ++i) {
            if (!connected[i]) continue;

            for (int j = 0; j < roomSize; ++j) {
                if (connected[j]) continue;

                int dist = std::abs(rooms[i].position.x - rooms[j].position.x)
                         + std::abs(rooms[i].position.y - rooms[j].position.y);

                if (dist < bestDist) {
                    bestDist = dist;
                    bestA = i;
                    bestB = j;
                }
            }
        }
        edges.push_back({bestA, bestB});
        connected[bestB] = true;
        connectedCount++;
    }
}

dg::Dungeon dg::DungeonGenerator::createDungeonFromData() {
    return Dungeon(rooms, edges);
}
