#include "Theme.hpp"

DungeonTheme DungeonThemeBuilder::generateDungeonTheme() {
    setVariables();
    generateTheme();
    generateDungeon();
    return dungeonTheme;
}

void DungeonThemeBuilder::setVariables() {
    size = std::size(bb::wallTopTiles);

    wallNumber = rnd::Get64() % size;
    floorNumber = rnd::Get64() % size;

    floorDecorationNumber = rnd::Get64() % size;
    floorDecorationNumber = floorDecorationNumber < 2 ? 3 : floorDecorationNumber;
}

void DungeonThemeBuilder::generateTheme() {
    wallSet = {
        bb::wallTopTiles[wallNumber],
        bb::wallSideTiles[wallNumber]
    };

    std::set<uint16_t> decorationsSet;
    while (decorationsSet.size() <= floorDecorationNumber) {
        decorationsSet.insert(bb::floorDecorationTiles[rnd::Get64() % size]);
    }
    floorSet = {
        bb::floorTiles[floorNumber],
        std::vector<uint16_t>(decorationsSet.begin(), decorationsSet.end())
    };
}

void DungeonThemeBuilder::generateDungeon() {
    dungeonTheme = DungeonTheme{
        wallSet,
        floorSet
    };
}