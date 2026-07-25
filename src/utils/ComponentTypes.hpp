#pragma once
#include <SDL3/SDL_surface.h>

struct TileTagComponent {};

struct PlayerTagComponent{};

struct EnemyTagComponent{};

struct RotatedComponent {
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
};