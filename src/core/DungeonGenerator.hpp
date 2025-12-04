#pragma once
#include <vector>

#include "Dungeon.hpp"
#include "Random.hpp"

namespace dg {
    class DungeonGenerator {
    public:
        DungeonGenerator();
        Dungeon generateDungeon();
    private:
        short roomCount = 0;
        std::vector<Room> rooms;
        std::vector<Edge> edges;

    private:
        void prepare();
        void createRooms();
        void fixRooms();
        void connectRooms();
        Dungeon createDungeonFromData();
    };

}