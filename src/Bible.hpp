#pragma once
#include <string>

namespace bb {
    struct Resource {
        std::string name;
        std::string path;
    };
    const Resource rogueRes = {"rogues", "./res/images/rogues.png"};
    const Resource tileRes = {"tiles", "./res/images/tiles.png"};

    constexpr float ASSETS_TILE_SIZE = 32.0f;

    constexpr float ROOM_SIZE = 100.0f;
    constexpr uint32_t TILES_PER_ROOM = 10;
    constexpr float TILE_SIZE = ROOM_SIZE / TILES_PER_ROOM;
}
