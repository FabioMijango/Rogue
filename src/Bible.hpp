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
    namespace Anim {
        constexpr std::string ID_PLAYER = "player";

        constexpr std::string ID_ENEMY = "enemy";

        constexpr std::string ID_TOP_WALL = "topWall";
        constexpr std::string ID_SIDE_WALL = "sideWall";

        constexpr std::string ID_FLOOR = "floor";
    }

    // Tags ID
    namespace Tags {
        constexpr std::string PLAYER = "Player";
        constexpr std::string WALL = "Wall";
    }

    // Actions name
    namespace Acts {
        constexpr std::string UP = "UP";
        constexpr std::string DOWN = "DOWN";
        constexpr std::string LEFT = "LEFT";
        constexpr std::string RIGHT = "RIGHT";

        constexpr std::string ZOOM = "ZOOM";
        constexpr std::string SPACE = "SPACE";
        constexpr std::string LEFT_CLICK = "LEFT_CLICK";
    }
}
