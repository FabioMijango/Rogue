#pragma once
#include "Bible.hpp"
#include "EntityManager.hpp"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_stdinc.h"
#include "Types.hpp"
#include "utils/ComponentTypes.hpp"
#include "utils/Utils.hpp"
#include <random>
#include <vector>

namespace sFSM {
inline void changeState(FSMComponent* component, const FSMComponent::State newState) {
    component->previousState = component->currentState;
    component->currentState = newState;

    component->newState = true;
}

inline void initState(FSMComponent* component, EntityManager& entityManager, Entity entity, Uint64 now) {

    switch (component->currentState) {
    case FSMComponent::State::IDLE:
        break;

    case FSMComponent::State::WALK: {
        const auto& walkData = component->stateData.walkStateData;
        auto* kinematic = entityManager.getComponent<KinematicComponent>(entity);

        kinematic->velocity = {0.0f, 0.0f};

        if (walkData.right) {
            kinematic->velocity.x += -bb::PLAYER_SPEED;
        }
        if (walkData.left) {
            kinematic->velocity.x += bb::PLAYER_SPEED;
        }
        if (walkData.up) {
            kinematic->velocity.y += -bb::PLAYER_SPEED;
        }
        if (walkData.down) {
            kinematic->velocity.y += bb::PLAYER_SPEED;
        }
        if (component->previousState != FSMComponent::State::WALK) {
            now -= bb::MOVEMENT_TIMEOUT;
        }
        break;
    }

    case FSMComponent::State::ATTACK: {
        if (now - component->stateInitialTime.timestamp < bb::ATTACK_TIMEOUT ) {
                return;
        }
        const auto& attackData = component->stateData.attackStateData;

        auto* transform = entityManager.getComponent<TransformComponent>(entity);

        const SDL_FPoint dir = { attackData.targetPosition.x - transform->position.x, attackData.targetPosition.y - transform->position.y };
        auto dirNorm = utils::normalize(dir);
        dirNorm.x *= utils::SQR_2 * bb::TILE_SIZE;
        dirNorm.y *= utils::SQR_2 * bb::TILE_SIZE;
        dirNorm.x += transform->position.x;
        dirNorm.y += transform->position.y;

        auto attackEntity = entityManager.createEntity();
        entityManager.addComponent<InheritanceComponent>(attackEntity, entity);
        entityManager.addComponent<TransformComponent>(attackEntity, dirNorm, SDL_FPoint{1.0f, 1.0f});
        entityManager.addComponent<BoxColliderComponent>(attackEntity, SDL_FPoint{bb::TILE_SIZE, bb::TILE_SIZE}, SDL_FPoint{0, 0});
        entityManager.addComponent<LifetimeComponent>(attackEntity, TimeComponent{ now }, bb::HITBOX_LIFETIME);
        entityManager.addComponent<AttackHitboxTagComponent>(attackEntity);
        entityManager.addComponent<EntitiesHittedComponent>(attackEntity, std::vector<Entity>{} );

        break;
    }

    case FSMComponent::State::PATROL:
        break;


    case FSMComponent::State::INVALID:
        break;

    }

    component->stateInitialTime.timestamp = now;
    component->newState = false;
}

inline void updateState(FSMComponent* component, EntityManager& entityManager, Entity entity, const Uint64 now) {

    switch (component->currentState) {
    case FSMComponent::State::IDLE:
        break;

    case FSMComponent::State::WALK: {
        if (now - component->stateInitialTime.timestamp < bb::MOVEMENT_TIMEOUT) {
            break;
        }

        auto* transform = entityManager.getComponent<TransformComponent>(entity);
        auto* kinematic = entityManager.getComponent<KinematicComponent>(entity);
        auto* rotation = entityManager.getComponent<RotatedComponent>(entity);

        transform->position.x += kinematic->velocity.x;
        transform->position.y += kinematic->velocity.y;
        rotation->flipMode = (kinematic->velocity.x > 0.0f) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        if (kinematic->velocity.x == 0.0f && kinematic->velocity.y == 0.0f) {
            changeState(component, FSMComponent::State::IDLE);
        }

        if (entityManager.getComponent<EnemyTagComponent>(entity) ) {
            changeState(component, FSMComponent::State::IDLE);
        }
        component->newState = false;
        component->stateInitialTime.timestamp = now;
        break;
    }

    case FSMComponent::State::ATTACK: {
        if (now - component->stateInitialTime.timestamp < bb::ATTACK_TIMEOUT) {
            break;
        }

        if (component->previousState == FSMComponent::State::WALK) {
            changeState(component, FSMComponent::State::WALK);
        } else {
            changeState(component, FSMComponent::State::IDLE);
        }
        break;
    }

    case FSMComponent::State::PATROL:
        break;


    case FSMComponent::State::INVALID:
        break;

    }
}


inline void setEnemyDirection(FSMComponent& fsmComponent, SDL_FPoint difference) {
    auto& walkData = fsmComponent.stateData.walkStateData;
    walkData = {false, false, false, false};
    if (difference.x > 0 ) {
        walkData.left = true;
    } else if ( difference.x < 0 ) {
        walkData.right = true;
    }

    if (difference.y > 0 ) {
        walkData.down = true;
    } else if ( difference.y < 0 ) {
        walkData.up = true;
    }
}

static std::mt19937 gen;
static std::uniform_int_distribution<> dirDistribution;

inline void updateStateEnemies(EntityManager& entityManager, const Uint64 now ) {
    Entity playerKey = entityManager.getSparseSet<PlayerTagComponent>().getKeys().front();
    auto playerPos = entityManager.getComponent<TransformComponent>(playerKey)->position;

    auto& enemyKeys = entityManager.getSparseSet<EnemyTagComponent>().getKeys();
    for (auto key : enemyKeys) {
        auto enemyPos = entityManager.getComponent<TransformComponent>(key)->position;

        auto* fsmComponent = entityManager.getComponent<FiniteStateMachineComponent>(key);
        if (  now - fsmComponent->stateInitialTime.timestamp < bb::MOVEMENT_TIMEOUT * 1.5f ) {
            continue;
        }
        auto difference = utils::difference( playerPos, enemyPos);
        if ( utils::lenght(difference) < bb::ENEMY_DETECTION_RADIUS) {
            setEnemyDirection(*fsmComponent, difference);
            changeState(fsmComponent, FSMComponent::State::WALK);
            initState(fsmComponent, entityManager, key, now);
        }

         updateState(fsmComponent, entityManager, key, now);
    }
}


} // namespace sFSM
