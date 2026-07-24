#include "GameScene.hpp"

#include "Bible.hpp"
#include "utils/AssetsUtils.hpp"
#include "utils/ComponentTypes.hpp"
#include "utils/DungeonGenerator.hpp"

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

    player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, SDL_FPoint{0, 0}, SDL_FPoint{0, 0});
    entityManager.addComponent<CameraComponent>(player, SDL_FPoint{bb::TILE_SIZE / 2.f, bb::TILE_SIZE / 2.f}, 1.f, SDL_FPoint{static_cast<float>(bb::WIDTH), static_cast<float>(bb::HEIGHT)});
    entityManager.addComponent<KinematicComponent>(player, SDL_FPoint{ 0.0f, 0.0f }, SDL_FPoint{ 0.0f, 0.0f });
    entityManager.addComponent<PlayerRotatedComponent>(player, false);
    entityManager.addComponent<PlayerTagComponent>(player);

    dungeon = DungeonGenerator().generate(entityManager);

    return true;
}

SDL_AppResult GameScene::update(float deltaTime) {
    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    auto* camera = entityManager.getComponent<CameraComponent>(player);

    transform->position.x += kinematic->velocity.x * deltaTime;
    transform->position.y += kinematic->velocity.y * deltaTime;

    camera->position.x = transform->position.x + bb::TILE_SIZE / 2.0f;
    camera->position.y = transform->position.y + bb::TILE_SIZE / 2.0f;


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
    const float SPEED = 100.f;
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    if (action.state == Action::State::Pressed) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += -SPEED;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += SPEED;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += -SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += SPEED;
        }
        if (action.name == "SPACE") {
            dungeon = DungeonGenerator().generate(entityManager);
        }
    }
    else if (action.state == Action::State::Released) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += SPEED;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += -SPEED;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += -SPEED;
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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    renderTiles(renderer, camera, screenSize);
    renderEnemies(renderer, camera, screenSize);
    renderPlayer(renderer, camera, screenSize);

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

    // TODO: Remove
    //  Render the box colliders for debugging
    auto tileIds = entityManager.getSparseSet<TileTagComponent>().getKeys();
    for (const auto& id : tileIds) {
        const auto b = entityManager.getComponent<BoxColliderComponent>(id);
        const auto t = entityManager.getComponent<TransformComponent>(id);

        auto [ x, y ] = sCamera::worldToScreen({t->position.x, t->position.y}, *camera);
        SDL_FRect tileBounds = {x, y, screenSize.x, screenSize.y};
        SDL_RenderRect(renderer, &tileBounds);
    }
}

void GameScene::renderEnemies(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint& screenSize) {
    auto& enemyAnim = Assets::Instance().getAnimation(bb::ANIMID_ENEMY);
    auto enemySprite = enemyAnim.getSprite();

    auto enemyIds = entityManager.getSparseSet<EnemyTagComponent>().getKeys();
    for (const auto& id : enemyIds) {
        const auto b = entityManager.getComponent<BoxColliderComponent>(id);
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

    SDL_RenderTexture(renderer, playerAnim.getTexture(), &playerSprite.m_textureRect, &playerBounds);
}
