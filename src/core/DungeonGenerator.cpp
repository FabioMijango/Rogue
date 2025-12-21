#include "DungeonGenerator.hpp"

dg::Dungeon dg::DungeonGenerator::generateDungeon(ResourceManager &resourceManager) {
    prepare();
    createRooms();
    fixRooms();
    connectRooms();
    setTileMaps();
    setTileMapsCache(resourceManager);
    initializeEntities(resourceManager);
    createDungeonFromData();
    return std::move(dungeon);
}

void dg::DungeonGenerator::prepare() {
    rooms.clear();
    roomCount = rnd::GetInt() % 3 + 10;

    DungeonThemeBuilder themeBuilder;
    theme = themeBuilder.generateDungeonTheme();
}

void dg::DungeonGenerator::createRooms() {
    Room initialRoom({0,0}, {10,10});
    rooms.push_back(initialRoom);

    for (size_t i = 0; i <= roomCount; i++) {
        Room newRoom(
            sf::Vector2f(rnd::GetInt() % 50, rnd::GetInt() % 50),
            sf::Vector2u(rnd::Get64() % 15 + 10, rnd::Get64() % 15 + 10)
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

void dg::DungeonGenerator::setTileMaps() {
    for (auto& room :rooms) {
        // Init vector size X, with vector size Y inside, filled with floor baseId
        std::vector<std::vector<uint16_t>> tileMap(room.size.x, std::vector<uint16_t>(room.size.y, theme.floor.baseId));
        for (size_t x = 0; x < room.size.x; x++) {
            for (size_t y = 0; y < room.size.y; y++) {
                if (x == 0 || x == room.size.x - 1)
                    tileMap[x][y] = theme.wall.topId;
                else if (y == 0 || y == room.size.y - 1)
                    tileMap[x][y] = theme.wall.sideId;

                if ( x == 0 && y == room.size.y - 1)
                    tileMap[x][y] = theme.wall.sideId;
                else if ( x == room.size.x - 1 && y == room.size.y - 1)
                    tileMap[x][y] = theme.wall.sideId;
            }
        }
        room.tileMap = tileMap;
    }
}

void dg::DungeonGenerator::setTileMapsCache(ResourceManager &resourceManager) {
    const std::weak_ptr<Atlas> atlas = resourceManager.getAtlas(bb::imageResources[0].name);

    tileCache.insert({theme.wall.topId, atlas.lock()->getSprite(theme.wall.topId)});
    tileCache.insert({theme.wall.sideId, atlas.lock()->getSprite(theme.wall.sideId)});
    tileCache.insert({theme.floor.baseId, atlas.lock()->getSprite(theme.floor.baseId)});
    for (auto id : theme.floor.decorations) {
        tileCache.insert({id, atlas.lock()->getSprite(id)});
    }

}

void dg::DungeonGenerator::initializeEntities(ResourceManager &resourceManager) {
    const sf::Sprite playerSprite = resourceManager.getAtlas("rogues").lock()->getSprite(0);
    // TODO: Logic for enemies sprites ...
    entityManager.init(playerSprite, &rooms);
}

void dg::DungeonGenerator::createDungeonFromData() {
    dungeon = Dungeon(rooms, edges, tileCache, std::move(entityManager));
    rooms.clear();
    edges.clear();
}
