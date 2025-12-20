#pragma once
#include <climits>
#include <vector>

#include "Dungeon.hpp"
#include "Random.hpp"
#include "ResourceManager.hpp"
#include "../bible/Theme.hpp"

namespace dg {
    class DungeonGenerator {
    public:
        DungeonGenerator() = default;
        Dungeon generateDungeon(ResourceManager &resourceManager);
    private:
        Dungeon dungeon;
        short roomCount = 0;
        std::vector<Room> rooms;
        std::vector<Edge> edges;
        std::unordered_map<uint16_t, sf::Sprite> tileCache;
        DungeonTheme theme;
        EntityManager entityManager;

    private:
        void prepare();
        void createRooms();
        void fixRooms();
        void connectRooms();
        void setTileMaps();
        void setTileMapsCache(ResourceManager &resourceManager);
        void createDungeonFromData();
        void initializeEntities(ResourceManager &resourceManager);
    };

}