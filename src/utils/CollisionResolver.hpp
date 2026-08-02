#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_rect.h>

#include "../Bible.hpp"
#include "EntityManager.hpp"
#include "Types.hpp"
#include "ComponentTypes.hpp"

namespace sPhysics {

// TODO: Implement more reliable collision resolution. This doesn't work correctly
inline void resolverPlayerTileCollision(EntityManager& entityManager, Entity entityA, Entity entityB, SDL_FRect& rectA, SDL_FRect& rectB) {
    auto* kinematicA = entityManager.getComponent<KinematicComponent>(entityA);
    auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
    auto* kinematicB = entityManager.getComponent<KinematicComponent>(entityB);
    auto* transformB = entityManager.getComponent<TransformComponent>(entityB);

    float overlapX = std::min(rectA.x + rectA.w - rectB.x, rectB.x + rectB.w - rectA.x);
    float overlapY = std::min(rectA.y + rectA.h - rectB.y, rectB.y + rectB.h - rectA.y);

    bool resolveX = overlapX < overlapY;
    float overlap = resolveX ? overlapX : overlapY;

    float dir = resolveX ? ((rectA.x < rectB.x) ? -1.0f : 1.0f) 
        : ((rectA.y < rectB.y) ? -1.0f : 1.0f);

    float pushA = (kinematicA && kinematicB) ? (dir * overlap * 0.5f) : (kinematicA ? dir * overlap : 0.0f);
    float pushB = (kinematicA && kinematicB) ? (-dir * overlap * 0.5f) : (kinematicB ? -dir * overlap : 0.0f);

    if (resolveX) {
        transformA->position.x += pushA;
        transformB->position.x += pushB;
    } else {
        transformA->position.y += pushA;
        transformB->position.y += pushB;
    }
}

inline void resolverCollisionAttack(EntityManager& entityManager, Entity entityA, Entity entityB, Uint64 now ) {
    auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
    auto* transformB = entityManager.getComponent<TransformComponent>(entityB);
    auto* BoxColliderA = entityManager.getComponent<BoxColliderComponent>(entityA);
    auto* BoxColliderB = entityManager.getComponent<BoxColliderComponent>(entityB);

    SDL_FRect boundingBoxA = sPhysics::getBoundingBox(*transformA, *BoxColliderA);
    SDL_FRect boundingBoxB = sPhysics::getBoundingBox(*transformB, *BoxColliderB);
    if (!sPhysics::checkAABBCollision(boundingBoxA, boundingBoxB)) {
        return;
    }

    auto* hittedA = entityManager.getComponent<EntitiesHittedComponent>(entityA);
    auto  hittedB = entityManager.getComponent<EntitiesHittedComponent>(entityB);

    // TODO: Add logic with HealtComponent to check if the entity is alive before adding it to the hitted list
    if(hittedA) {
        for ( auto& entity : hittedA->entitiesHitted ) {
            if (entity == entityB) {
                return;
            }
        }
        hittedA->entitiesHitted.push_back(entityB);
        entityManager.addComponent<LifetimeComponent>(entityB, TimeComponent(now), bb::ATTACK_TIMEOUT );

    } else if (hittedB) {
        for ( auto& entity : hittedB->entitiesHitted ) {
            if (entity == entityA) {
                return;
            }
        }
        hittedB->entitiesHitted.push_back(entityA);
        entityManager.addComponent<LifetimeComponent>(entityA, TimeComponent(now), bb::ATTACK_TIMEOUT );
    }
}
}// namespace sPhysics
