#pragma once
#include <cstdint>
#include <set>
#include <vector>

#include "../bible/Bible.hpp"
#include "../core/Random.hpp"

struct WallSet {
    uint16_t topId;
    uint16_t sideId;
};
struct FloorSet {
    uint16_t baseId;
    std::vector<uint16_t> decorations;
};

struct DungeonTheme {
    WallSet wall;
    FloorSet floor;
};

class DungeonThemeBuilder {
public:
    DungeonThemeBuilder() = default;
    DungeonTheme generateDungeonTheme();
private:
    uint16_t size = 0;
    uint16_t wallNumber = 0;
    uint16_t floorNumber = 0;
    uint16_t floorDecorationNumber = 0;

    WallSet wallSet;
    FloorSet floorSet;

    DungeonTheme dungeonTheme;
private:
    void setVariables();
    void generateTheme();
    void generateDungeonThemeFromData();
};