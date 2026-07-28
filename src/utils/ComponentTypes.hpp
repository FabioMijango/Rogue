#pragma once

struct TileTagComponent {};

struct PlayerTagComponent{};

struct EnemyTagComponent{};

struct RotatedComponent {
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
};

struct HealthComponent {
    int health = -1;
};

struct InheritanceComponent {
    Entity parent = -1;
};

struct LifetimeComponent {
    TimeComponent time = {0};
    Uint64 lifetime = 0;
};

struct WalkStateData {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

struct AttackStateData {
    SDL_FPoint entityPosition = {0.0f, 0.0f};
    SDL_FPoint targetPosition = {0.0f, 0.0f};
};

union StateData {
    WalkStateData walkStateData = {};
    AttackStateData attackStateData;
};

struct FiniteStateMachineComponent {
    enum class State {
        IDLE,
        WALK,
        ATTACK,
        INVALID,
    };
    State currentState = State::IDLE;
    State previousState = State::INVALID;

    StateData stateData = {};

    TimeComponent stateInitialTime = {0};
    bool newState = false;
};
typedef FiniteStateMachineComponent FSMComponent;