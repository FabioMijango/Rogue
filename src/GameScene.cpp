#include "GameScene.hpp"

#include "Bible.hpp"
#include "utils/AssetsUtils.hpp"
#include "utils/ComponentTypes.hpp"
#include "utils/DungeonGenerator.hpp"
#include <algorithm>

#include "utils/Utils.hpp"

GameScene::~GameScene() = default;

bool GameScene::init(void** screenData) {
    if (!AssetsUtils::loadAssets()) {
        return false;
    }

    registerAction(SDL_SCANCODE_SPACE, "SPACE");
    registerAction(ScrollType::Vertical, "ZOOM");
    registerAction(SDL_SCANCODE_A, "RIGHT");
    registerAction(SDL_SCANCODE_D, "LEFT");
    registerAction(SDL_SCANCODE_W, "UP");
    registerAction(SDL_SCANCODE_S, "DOWN");
    registerAction(SDL_BUTTON_LEFT, "LEFT_CLICK");

    player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, SDL_FPoint{bb::ROOM_SIZE / 2.f, bb::ROOM_SIZE / 2.f}, SDL_FPoint{1.0f, 1.0f});
    entityManager.addComponent<BoxColliderComponent>(player, SDL_FPoint{bb::TILE_SIZE, bb::TILE_SIZE}, SDL_FPoint{0, 0});
    entityManager.addComponent<CameraComponent>(player, SDL_FPoint{bb::TILE_SIZE / 2.f, bb::TILE_SIZE / 2.f}, 1.f, SDL_FPoint{static_cast<float>(bb::WIDTH), static_cast<float>(bb::HEIGHT)});
    entityManager.addComponent<KinematicComponent>(player, SDL_FPoint{ 0.0f, 0.0f }, SDL_FPoint{ 0.0f, 0.0f });
    entityManager.addComponent<RotatedComponent>(player, SDL_FLIP_NONE);
    entityManager.addComponent<PlayerTagComponent>(player);
    entityManager.addComponent<HealthComponent>(player, 5);
    entityManager.addComponent<AttackComponent>(player, true, SDL_GetTicks());
    entityManager.addComponent<TimeComponent>(player, SDL_GetTicks());

    dungeon = DungeonGenerator().generate(entityManager);

    return true;
}

SDL_AppResult GameScene::update(float deltaTime) {
    for (auto& entity : entitiesToDestroy) {
        entityManager.destroyEntity(entity);
    }
    entitiesToDestroy.clear();

    Uint64 now = SDL_GetTicks();

    auto* transformPlayer = entityManager.getComponent<TransformComponent>(player);
    auto* kinematicPlayer = entityManager.getComponent<KinematicComponent>(player);
    auto* timerPlayer = entityManager.getComponent<TimeComponent>(player);
    auto* attackPlayer = entityManager.getComponent<AttackComponent>(player);
    auto* camera = entityManager.getComponent<CameraComponent>(player);

    auto movement = kinematicPlayer->velocity.x != 0.0f || kinematicPlayer->velocity.y != 0.0f;
    if (now - timerPlayer->timestamp >= bb::MOVEMENT_TIMEOUT && movement) {
        transformPlayer->position.x += kinematicPlayer->velocity.x;
        transformPlayer->position.y += kinematicPlayer->velocity.y;

        timerPlayer->timestamp = now;
    }

    if (attackPlayer->isAttacking && now - attackPlayer->attackTimer.timestamp >= bb::ATTACK_TIMEOUT) {

        SDL_FPoint mousePosInWorld = sCamera::screenToWorld(mousePosition, *camera);

        SDL_FPoint dir = { mousePosInWorld.x - transformPlayer->position.x, mousePosInWorld.y - transformPlayer->position.y };
        auto dirNorm = utils::normalize(dir);
        dirNorm.x *= utils::SQR_2 * bb::TILE_SIZE;
        dirNorm.y *= utils::SQR_2 * bb::TILE_SIZE;
        dirNorm.x += transformPlayer->position.x;
        dirNorm.y += transformPlayer->position.y;

        auto attackEntity = entityManager.createEntity();
        entityManager.addComponent<InheritanceComponent>(attackEntity, player);
        entityManager.addComponent<TransformComponent>(attackEntity, dirNorm, SDL_FPoint{1.0f, 1.0f});
        entityManager.addComponent<BoxColliderComponent>(attackEntity, SDL_FPoint{bb::TILE_SIZE, bb::TILE_SIZE}, SDL_FPoint{0, 0});
        entityManager.addComponent<LifetimeComponent>(attackEntity, now, bb::HITBOX_LIFETIME);

        attackPlayer->attackTimer.timestamp = now;
    }

    spatialGrid.populateMap();
    auto potencialCollisions = spatialGrid.getPotentialCollisions();

    for (auto& [entityA, entityB] : potencialCollisions) {
        auto* transformA = entityManager.getComponent<TransformComponent>(entityA);
        auto* transformB = entityManager.getComponent<TransformComponent>(entityB);
        auto* colliderA = entityManager.getComponent<BoxColliderComponent>(entityA);
        auto* colliderB = entityManager.getComponent<BoxColliderComponent>(entityB);

        SDL_FRect rectA = sPhysics::getBoundingBox(*transformA, *colliderA);
        SDL_FRect rectB = sPhysics::getBoundingBox(*transformB, *colliderB);
        
        if (sPhysics::checkAABBCollision(rectA, rectB)) {
            auto* kinematicA = entityManager.getComponent<KinematicComponent>(entityA);
            auto* kinematicB = entityManager.getComponent<KinematicComponent>(entityB);

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
    }

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
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    auto* rotation = entityManager.getComponent<RotatedComponent>(player);
    auto* attack = entityManager.getComponent<AttackComponent>(player);

    if (action.state == Action::State::Pressed) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += -bb::PLAYER_SPEED;
            rotation->flipMode = SDL_FLIP_NONE;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += bb::PLAYER_SPEED;
            rotation->flipMode = SDL_FLIP_HORIZONTAL;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += -bb::PLAYER_SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += bb::PLAYER_SPEED;
        }
        if (action.name == "SPACE") {
            dungeon = DungeonGenerator().generate(entityManager);
        }
        if (action.name == "LEFT_CLICK") {
            attack->isAttacking = true;
            mousePosition = {action.x, action.y};
        }
    }
    else if (action.state == Action::State::Released) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += bb::PLAYER_SPEED;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += -bb::PLAYER_SPEED;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += bb::PLAYER_SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += -bb::PLAYER_SPEED;
        }
    } else if (action.state == Action::State::Vertical_Scroll) {
        auto* camera = entityManager.getComponent<CameraComponent>(player);
        camera->zoom += action.y * 0.1f;
        if (camera->zoom < 0.1f) {
            camera->zoom = 0.1f;
        }
    }
}

void GameScene::sRender(SDL_Renderer *renderer) {
    auto* camera = entityManager.getComponent<CameraComponent>(player);
    const SDL_FPoint screenSize = { bb::TILE_SIZE * camera->zoom, bb::TILE_SIZE * camera->zoom };

    SDL_SetRenderDrawColor(renderer, 12, 19, 23, 255);
    SDL_RenderClear(renderer);

    renderTiles(renderer, camera, screenSize);
    renderEnemies(renderer, camera, screenSize);
    renderPlayer(renderer, camera, screenSize);

    renderCollisionBoxes(renderer, camera, screenSize);

    SDL_RenderPresent(renderer);
}

void GameScene::exit() {
}

std::shared_ptr<Scene> GameScene::changeScene() {
    return nullptr;
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

void GameScene::renderEnemies(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto& enemyAnim = Assets::Instance().getAnimation(bb::ANIMID_ENEMY);
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
    auto& playerAnim = Assets::Instance().getAnimation(bb::ANIMID_PLAYER);
    auto playerSprite = playerAnim.getSprite();

    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto [ x, y ] =  sCamera::worldToScreen({transform->position.x, transform->position.y}, *camera);
    SDL_FRect playerBounds = {x, y, screenSize.x, screenSize.y};

    auto* rotation = entityManager.getComponent<RotatedComponent>(player);

    SDL_RenderTextureRotated(renderer, playerAnim.getTexture(), &playerSprite.m_textureRect, &playerBounds, 0.0, nullptr, rotation->flipMode);
}

void GameScene::renderCollisionBoxes(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    auto tileIds = entityManager.getSparseSet<BoxColliderComponent>().getKeys();
    for (const auto& id : tileIds) {
        // const auto b = entityManager.getComponent<BoxColliderComponent>(id);
        const auto t = entityManager.getComponent<TransformComponent>(id);

        auto [ x, y ] = sCamera::worldToScreen({t->position.x, t->position.y}, *camera);
        SDL_FRect tileBounds = {x, y, screenSize.x, screenSize.y};
        SDL_RenderRect(renderer, &tileBounds);
    }
}

void GameScene::destroyExpiredEntities(Uint64 now) {
    auto& lifetimeKeys = entityManager.getSparseSet<LifetimeComponent>().getKeys();
    for (auto& entity : lifetimeKeys) {
        auto* lifetimeComponent = entityManager.getComponent<LifetimeComponent>(entity);
        if (now - lifetimeComponent->time.timestamp >= lifetimeComponent->lifetime) {
            auto* inheritanceComponent = entityManager.getComponent<InheritanceComponent>(entity);
            if (inheritanceComponent) {
                auto* parentAttack = entityManager.getComponent<AttackComponent>(inheritanceComponent->parent);
                parentAttack->isAttacking = false;
                parentAttack->attackTimer.timestamp = now;
            }
            entitiesToDestroy.push_back(entity);
        }
    }
}
