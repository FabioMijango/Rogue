#pragma once
#include <string>

namespace bb {
    struct Resource {
        std::string name;
        std::string path;
    };
    const Resource rogueRes = {"rogues", "./res/images/rogues.png"};
    const Resource monsterRes = {"monsters", "./res/images/monsters.png"};
    const Resource tileRes = {"tiles", "./res/images/tiles.png"};

    // Window config
    constexpr int WIDTH = 1920;
    constexpr int HEIGHT = 1080;

    // Assets config
    constexpr float ASSETS_TILE_SIZE = 32.0f;

    // Dungeon creation config
    constexpr int NUMBER_OF_ROOMS = 5;
    constexpr float ROOM_SIZE = 500.0f;
    constexpr uint32_t TILES_PER_ROOM = 10;
    constexpr float TILE_SIZE = ROOM_SIZE / TILES_PER_ROOM;

    constexpr float PROB_TO_BACK_TO_START = 0.4f;
    constexpr float PROB_TO_SPAWN_ENEMY = 0.02f;

    // Player config
    constexpr float PLAYER_SPEED = TILE_SIZE;

    constexpr Uint64 MOVEMENT_TIMEOUT = 500.f;
    constexpr Uint64 ATTACK_TIMEOUT = 500.f;
    constexpr Uint64 HITBOX_LIFETIME = 500.f;

    // Animations ID
    constexpr std::string ANIMID_PLAYER = "player";

    constexpr std::string ANIMID_ENEMY = "enemy";

    constexpr std::string ANIMID_TOP_WALL = "topWall";
    constexpr std::string ANIMID_SIDE_WALL = "sideWall";

    constexpr std::string ANIMID_FLOOR = "floor";

    // Tags ID
    constexpr std::string TAG_PLAYER = "Player";
    constexpr std::string TAG_WALL = "Wall";
}
