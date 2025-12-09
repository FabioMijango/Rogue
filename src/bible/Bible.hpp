#pragma once

#include <string>

namespace bb {
    struct Resource {
        std::string name;
        std::string imagePath;
        std::string metaPath;
    };

    inline const Resource resources[] = {
        {"tiles", "./res/tiles.png", "./res/tiles.txt"},
    };

    inline const uint16_t wallTopTiles[] = { 0, 2, 4, 6, 8, 10};
    inline const uint16_t wallSideTiles[] = { 1, 3, 5, 7, 9, 11};
    inline const uint16_t floorTiles[] = { 12, 19, 47, 54, 58, 66};
    inline const uint16_t floorDecorationTiles[] =
        { 16, 17, 18,
        23, 24, 25,
        30, 31, 32,
        37, 38, 39,
        44, 45, 46,
        51, 52, 53};
}
