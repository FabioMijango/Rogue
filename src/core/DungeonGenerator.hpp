#pragma once
#include <climits>
#include <vector>

#include "Dungeon.hpp"
#include "Random.hpp"
#include "../bible/Theme.hpp"

namespace dg {
    class DungeonGenerator {
    public:
        DungeonGenerator() = default;
        Dungeon generateDungeon();
    private:
        short roomCount = 0;
        std::vector<Room> rooms;
        std::vector<Edge> edges;
        DungeonTheme theme;

    private:
        void prepare();
        void createRooms();
        void fixRooms();
        void connectRooms();
        void setTileMaps();
        Dungeon createDungeonFromData();
    };

}