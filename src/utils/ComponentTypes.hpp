#pragma once
#include <SDL3/SDL_surface.h>

struct TileTagComponent {};

struct PlayerTagComponent{};

struct EnemyTagComponent{};

struct RotatedComponent {
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
};

struct HealthComponent {
    int health = -1;
};

struct AttackComponent {
    bool isAttacking = false;

    TimeComponent attackTimer = {0};
};

struct InheritanceComponent {
    Entity parent = -1;
};

struct LifetimeComponent {
    TimeComponent time = {0};
    Uint64 lifetime = 0;
};