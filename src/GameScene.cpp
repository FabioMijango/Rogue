#include "GameScene.hpp"

#include <SDL3/SDL_stdinc.h>
#include <memory>

#include "Assets.hpp"
#include "Bible.hpp"
#include "CameraUtils.hpp"
#include "FiniteStateMachineSystem.hpp"
#include "PhysicsUtils.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "utils/AssetsUtils.hpp"
#include "utils/ComponentTypes.hpp"
#include "utils/DungeonGenerator.hpp"
#include "utils/CollisionResolver.hpp"
#include "InitScene.hpp"

bool GameScene::init(void** screenData) {
    if (!AssetsUtils::loadAssets()) {
        return false;
    }

    // registerAction(SDL_SCANCODE_SPACE, bb::Acts::SPACE);
    // registerAction(ScrollType::Vertical, bb::Acts::ZOOM);
    registerAction(SDL_SCANCODE_A, bb::Acts::RIGHT);
    registerAction(SDL_SCANCODE_D, bb::Acts::LEFT);
    registerAction(SDL_SCANCODE_W, bb::Acts::UP);
    registerAction(SDL_SCANCODE_S, bb::Acts::DOWN);
    registerAction(SDL_BUTTON_LEFT, bb::Acts::LEFT_CLICK);

    player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, SDL_FPoint{bb::ROOM_SIZE / 2.f, bb::ROOM_SIZE / 2.f}, SDL_FPoint{1.0f, 1.0f});
    entityManager.addComponent<BoxColliderComponent>(player, SDL_FPoint{bb::TILE_SIZE, bb::TILE_SIZE}, SDL_FPoint{0, 0});
    entityManager.addComponent<CameraComponent>(player, SDL_FPoint{bb::TILE_SIZE / 2.f, bb::TILE_SIZE / 2.f}, 1.f, SDL_FPoint{static_cast<float>(bb::WIDTH), static_cast<float>(bb::HEIGHT)});
    entityManager.addComponent<KinematicComponent>(player, SDL_FPoint{ 0.0f, 0.0f }, SDL_FPoint{ 0.0f, 0.0f });
    entityManager.addComponent<FiniteStateMachineComponent>(player, FSMComponent::State::IDLE, FSMComponent::State::INVALID, StateData{}, SDL_GetTicks());
    entityManager.addComponent<RotatedComponent>(player, SDL_FLIP_NONE);
    entityManager.addComponent<PlayerTagComponent>(player);
    entityManager.addComponent<HealthComponent>(player, 5);

    dungeon = DungeonGenerator().generate(entityManager);

    return true;
}

SDL_AppResult GameScene::update(float deltaTime) {
    if (entityManager.getComponent<HealthComponent>(player)->health <= 0) {
        nextSceneType = NextScene::InitScene;
        m_hasEnded = true;
    }
    for (auto& entity : entitiesToDestroy) {
        entityManager.destroyEntity(entity);
    }
    entitiesToDestroy.clear();

    Uint64 now = SDL_GetTicks();

    auto* fsmPlayer = entityManager.getComponent<FiniteStateMachineComponent>(player);
    if (fsmPlayer->newState) {
        sFSM::initState(fsmPlayer, entityManager, player, now);
    }
    sFSM::updateState(fsmPlayer, entityManager, player, now);
    
    sFSM::updateStateEnemies(entityManager, now);

    spatialGrid.populateMap();

    auto collPlayerStair = spatialGrid.getPotentialCollisionBetween<PlayerTagComponent, StairsTagComponent>();
    for (auto& [entityA, entityB] : collPlayerStair) {
        auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
        auto* boxColliderA = entityManager.getComponent<BoxColliderComponent>(entityA);
        auto* transformB = entityManager.getComponent<TransformComponent>(entityB);
        auto* boxColliderB = entityManager.getComponent<BoxColliderComponent>(entityB);

        auto boundingBoxA = sPhysics::getBoundingBox(*transformA, *boxColliderA);
        auto boundingBoxB = sPhysics::getBoundingBox(*transformB, *boxColliderB);

        if (sPhysics::checkAABBCollision(boundingBoxA, boundingBoxB)) {
            nextSceneType = NextScene::NextLevel;
            m_hasEnded = true;
        }
    }

    auto collsPlayerTile = spatialGrid.getPotentialCollisionBetween<PlayerTagComponent, TileTagComponent>();
    for ( auto& [ entityA, entityB ] : collsPlayerTile ) {
        auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
        auto* boxColliderA = entityManager.getComponent<BoxColliderComponent>(entityA);
        auto* transformB = entityManager.getComponent<TransformComponent>(entityB);
        auto* boxColliderB = entityManager.getComponent<BoxColliderComponent>(entityB);

        auto boundingBoxA = sPhysics::getBoundingBox(*transformA, *boxColliderA);
        auto boundingBoxB = sPhysics::getBoundingBox(*transformB, *boxColliderB);

        if ( sPhysics::checkAABBCollision(boundingBoxA, boundingBoxB)) {
            sPhysics::resolverPlayerTileCollision(entityManager, entityA, entityB, boundingBoxA, boundingBoxB);
        }

    }

    auto collsAttackPlayerEnemy = spatialGrid.getPotentialCollisionBetween<AttackHitboxTagComponent, EnemyTagComponent>();
    for ( auto& [ entityA, entityB ] : collsAttackPlayerEnemy ) {
        sPhysics::resolverCollisionAttack(entityManager, entityA, entityB, now);
    }
    auto collsAttackEnemyPlayer = spatialGrid.getPotentialCollisionBetween<AttackHitboxTagComponent, PlayerTagComponent>();
    for (auto& [ entityA, entityB ] : collsAttackEnemyPlayer) {
        sPhysics::resolverCollisionAttack(entityManager, entityA, entityB, now);
    }

    auto* transformPlayer = entityManager.getComponent<TransformComponent>(player);
    auto* camera = entityManager.getComponent<CameraComponent>(player);
    camera->position.x = transformPlayer->position.x + bb::TILE_SIZE / 2.0f;
    camera->position.y = transformPlayer->position.y + bb::TILE_SIZE / 2.0f;

    destroyExpiredEntities(now);

    return SDL_APP_CONTINUE;
}

SDL_AppResult GameScene::eventHandler(const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }
    return SDL_APP_CONTINUE;
}

void GameScene::sDoAction(const Action &action) {
    auto* fsmComponent = entityManager.getComponent<FiniteStateMachineComponent>(player);

    FSMComponent::State newState = FSMComponent::State::INVALID;

    if (action.state == Action::State::Pressed) {
        if (action.name == bb::Acts::RIGHT) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.right = true;
        }
        if (action.name == bb::Acts::LEFT) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.left = true;
        }
        if (action.name == bb::Acts::UP) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.up = true;
        }
        if (action.name == bb::Acts::DOWN) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.down = true;
        }
        /*
        if (action.name == bb::Acts::SPACE) {
            dungeon = DungeonGenerator().generate(entityManager);
        }
        */
        if (action.name == bb::Acts::LEFT_CLICK) {
            newState = FSMComponent::State::ATTACK;
            auto* camera = entityManager.getComponent<CameraComponent>(player);
            fsmComponent->stateData.attackStateData.targetPosition = sCamera::screenToWorld({action.x - bb::TILE_SIZE / 2.f, action.y - bb::TILE_SIZE / 2.f }, *camera);
        }
    }
    else if (action.state == Action::State::Released) {
        if (action.name == bb::Acts::RIGHT) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.right = false;
        }
        if (action.name == bb::Acts::LEFT) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.left = false;
        }
        if (action.name == bb::Acts::UP) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.up = false;
        }
        if (action.name == bb::Acts::DOWN) {
            newState = FSMComponent::State::WALK;
            fsmComponent->stateData.walkStateData.down = false;
        }
    }
    /*
    else if (action.state == Action::State::Vertical_Scroll) {
        auto* camera = entityManager.getComponent<CameraComponent>(player);
        camera->zoom += action.y * 0.1f;
        if (camera->zoom < 0.1f) {
            camera->zoom = 0.1f;
        }
    }
    */

    if (newState != FSMComponent::State::INVALID) {
        sFSM::changeState(fsmComponent, newState);
    }
}

void GameScene::sRender(SDL_Renderer *renderer) {
    auto* camera = entityManager.getComponent<CameraComponent>(player);
    const SDL_FPoint screenSize = { bb::TILE_SIZE * camera->zoom, bb::TILE_SIZE * camera->zoom };

    SDL_SetRenderDrawColor(renderer, 12, 19, 23, 255);
    SDL_RenderClear(renderer);

    renderTiles(renderer, camera, screenSize);
    renderStair(renderer, camera, screenSize);
    renderEnemies(renderer, camera, screenSize);
    renderPlayer(renderer, camera, screenSize);
    renderHitsEntities(renderer, camera, screenSize);
    renderUI(renderer, camera, screenSize);

    // renderCollisionBoxes(renderer, camera, screenSize);

    SDL_RenderPresent(renderer);
}

void GameScene::exit() {
}

std::shared_ptr<Scene> GameScene::changeScene() {
    std::shared_ptr<Scene> nextScene = nullptr;
    if (nextSceneType == NextScene::NextLevel) {
        nextScene = std::make_shared<GameScene>();
    } else if (nextSceneType == NextScene::InitScene) {
        nextScene = std::make_shared<InitScene>();
    }
    nextScene->init(nullptr);
    return nextScene;
}

void GameScene::renderTiles(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    for (auto& room : dungeon.m_rooms | std::ranges::views::values) {
        for (auto& cell : room.cells) {

            auto& anim = cell.animation;
            auto sprite = anim->getSprite();

            SDL_FPoint screenPos = sCamera::worldToScreen({cell.tileBounds.x, cell.tileBounds.y }, *camera);
            SDL_FRect tileBounds = {screenPos.x, screenPos.y, screenSize.x, screenSize.y};

            SDL_RenderTexture(renderer, anim->getTexture(), &sprite.m_textureRect, &tileBounds);
        }
    }
}

void GameScene::renderStair(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto stairsKeys = entityManager.getSparseSet<StairsTagComponent>().getKeys();
    for (auto key : stairsKeys ) {
        auto* transform = entityManager.getComponent<TransformComponent>(key);
        auto* boxCollider = entityManager.getComponent<BoxColliderComponent>(key);

        auto& anim = Assets::Instance().getAnimation(bb::Anim::ID_STAIRS);
        auto sprite = anim.getSprite();

        SDL_FPoint screenPos = sCamera::worldToScreen( {transform->position.x, transform->position.y }, *camera);
        SDL_FRect stairsBounds = {screenPos.x, screenPos.y, boxCollider->size.x * camera->zoom, boxCollider->size.y * camera->zoom};

        SDL_RenderTexture(renderer, anim.getTexture(), &sprite.m_textureRect, &stairsBounds);
    }
}

void GameScene::renderEnemies(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto& enemyAnim = Assets::Instance().getAnimation(bb::Anim::ID_ENEMY);
    auto enemySprite = enemyAnim.getSprite();

    auto enemyIds = entityManager.getSparseSet<EnemyTagComponent>().getKeys();
    for (const auto& id : enemyIds) {
        // const auto b = entityManager.getComponent<BoxColliderComponent>(id);
        const auto t = entityManager.getComponent<TransformComponent>(id);

        auto [x, y] = sCamera::worldToScreen({t->position.x, t->position.y}, *camera);
        SDL_FRect enemyBounds = {x, y, screenSize.x, screenSize.y};
        SDL_RenderTexture(renderer, enemyAnim.getTexture(), &enemySprite.m_textureRect, &enemyBounds);
    }
}

void GameScene::renderPlayer(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto& playerAnim = Assets::Instance().getAnimation(bb::Anim::ID_PLAYER);
    auto playerSprite = playerAnim.getSprite();

    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto [ x, y ] =  sCamera::worldToScreen({transform->position.x, transform->position.y}, *camera);
    SDL_FRect playerBounds = {x, y, screenSize.x, screenSize.y};

    auto* rotation = entityManager.getComponent<RotatedComponent>(player);

    SDL_RenderTextureRotated(renderer, playerAnim.getTexture(), &playerSprite.m_textureRect, &playerBounds, 0.0, nullptr, rotation->flipMode);
}

void GameScene::renderHitsEntities(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto& assets = Assets::Instance();
    auto& hitAnim = assets.getAnimation(bb::Anim::ID_HIT);
    auto sprite = hitAnim.getSprite();

    auto hitEntities = entityManager.getSparseSet<AttackHitboxTagComponent>().getKeys();
    for (auto entity : hitEntities ) {
        auto* transform = entityManager.getComponent<TransformComponent>(entity);
        auto* collider = entityManager.getComponent<BoxColliderComponent>(entity);
        
        auto [ x, y ] =  sCamera::worldToScreen({transform->position.x, transform->position.y}, *camera);
        SDL_FRect bounds = { x, y, screenSize.x, screenSize.y };

        SDL_RenderTexture(renderer, hitAnim.getTexture(), &sprite.m_textureRect, &bounds);
    }
}

void GameScene::renderUI(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto* health = entityManager.getComponent<HealthComponent>(player);

    auto& anim = Assets::Instance().getAnimation(bb::heartRes.name);
    auto sprite = anim.getSprite();

    for (Uint32 i = 0; i < health->health; i++) {
        const SDL_FRect heartBound = { i * bb::ASSETS_TILE_SIZE * 2.f, 10, bb::ASSETS_TILE_SIZE * 2.f, bb::ASSETS_TILE_SIZE * 2.f };
        SDL_RenderTexture(renderer, anim.getTexture(), &sprite.m_textureRect, &heartBound);
    }
}

void GameScene::renderCollisionBoxes(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    auto tileIds = entityManager.getSparseSet<BoxColliderComponent>().getKeys();
    for (const auto& id : tileIds) {
        // const auto b = entityManager.getComponent<BoxColliderComponent>(id);
        const auto t = entityManager.getComponent<TransformComponent>(id);

        auto [ x, y ] = sCamera::worldToScreen({t->position.x, t->position.y}, *camera);
        SDL_FRect tileBounds = {x, y, screenSize.x, screenSize.y};
        const char* idChar = std::to_string(id).c_str();
        SDL_RenderRect(renderer, &tileBounds);
        SDL_RenderDebugText(renderer, x + 1, y + 1, idChar);
    }
}

void GameScene::destroyExpiredEntities(Uint64 now) {
    auto& lifetimeKeys = entityManager.getSparseSet<LifetimeComponent>().getKeys();

    for (auto key : lifetimeKeys ) {
        auto* lifetimeComponent = entityManager.getComponent<LifetimeComponent>(key);

        if ( now - lifetimeComponent->time.timestamp >= lifetimeComponent->lifetime ) {
            entitiesToDestroy.push_back(key);
        }
    }
}
