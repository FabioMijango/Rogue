#include "GameScene.hpp"

#include "Bible.hpp"
#include "utils/DungeonGenerator.hpp"

GameScene::~GameScene() = default;

bool GameScene::init() {
    Assets& assets = Assets::Instance();
    const SDL_Texture *tex = assets.loadTexture(bb::rogueRes.name, bb::rogueRes.path, false);
    if (!tex) return false;
    tex = assets.loadTexture(bb::tileRes.name, bb::tileRes.path, false);
    if (!tex) return false;

    Animation* anim =  assets.loadAnimation("player", bb::rogueRes.name,{ 0, 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!anim) return false;


    // Temp floor texture
    Animation* floorAnim =  assets.loadAnimation(bb::ANIMID_FLOOR, bb::tileRes.name, { 0 , 7 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!floorAnim) return false;

    // Temp wall texture
    Animation* sideWallAnim =  assets.loadAnimation(bb::ANIMID_SIDE_WALL, bb::tileRes.name, { 0 , 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!sideWallAnim) return false;

    Animation* topWallAnim =  assets.loadAnimation(bb::ANIMID_TOP_WALL, bb::tileRes.name, { 0 , 1 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!topWallAnim) return false;

    registerAction(SDL_SCANCODE_SPACE, "SPACE");
    registerAction(ScrollType::Vertical, "ZOOM");
    registerAction(SDL_SCANCODE_A, "RIGHT");
    registerAction(SDL_SCANCODE_D, "LEFT");
    registerAction(SDL_SCANCODE_W, "UP");
    registerAction(SDL_SCANCODE_S, "DOWN");

    player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, SDL_FPoint{0, 0}, SDL_FPoint{0, 0});
    entityManager.addComponent<CameraComponent>(player, SDL_FPoint{0, 0}, 1.f, SDL_FPoint{bb::HEIGHT, bb::WIDTH});
    entityManager.addComponent<KinematicComponent>(player, SDL_FPoint{ 0.0f, 0.0f }, SDL_FPoint{ 0.0f, 0.0f });

    dungeon = DungeonGenerator().generate(entityManager);

    return true;
}

SDL_AppResult GameScene::update(float deltaTime) {
    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    auto* camera = entityManager.getComponent<CameraComponent>(player);

    auto deltaX = kinematic->velocity.x * deltaTime;
    auto deltaY = kinematic->velocity.y * deltaTime;
    transform->position.x += deltaX;
    transform->position.y += deltaY;
    camera->position.x += deltaX;
    camera->position.y += deltaY;


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
        // TODO: Fix camera movement when zoom occurs
        if (camera->zoom < 0.1f) {
            camera->zoom = 0.1f;
        }
    }
}

void GameScene::sRender(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    const Animation& playerAnim = Assets::Instance().getAnimation("player");
    const auto rect = playerAnim.getSprite().m_textureRect;
    SDL_RenderTexture(renderer, playerAnim.getTexture(), &rect, nullptr);

    auto* camera = entityManager.getComponent<CameraComponent>(player);
    float screenW = bb::TILE_SIZE * camera->zoom;
    float screenH = bb::TILE_SIZE * camera->zoom;

    for (auto& room : dungeon.m_rooms | std::ranges::views::values) {
        for (auto& cell : room.cells) {

            auto& anim = cell.animation;
            auto sprite = anim->getSprite();

            SDL_FPoint screenPos = sCamera::worldToScreen({cell.tileBounds.x, cell.tileBounds.y }, *camera);
            SDL_FRect tileBounds = {screenPos.x, screenPos.y, screenW, screenH};

            SDL_RenderTexture(renderer, anim->getTexture(), &sprite.m_textureRect, &tileBounds);
        }
    }

    // TODO: Remove
    //  Render the box colliders for debugging
    auto ids = entityManager.getSparseSet<BoxColliderComponent>().getKeys();
    for (const auto& id : ids) {
        const auto b = entityManager.getComponent<BoxColliderComponent>(id);
        const auto t = entityManager.getComponent<TransformComponent>(id);

        auto [ x, y ] = sCamera::worldToScreen({t->position.x, t->position.y}, *camera);
        SDL_FRect tileBounds = {x, y, b->width * camera->zoom, b->height * camera->zoom};
        SDL_RenderRect(renderer, &tileBounds);
    }

    SDL_RenderPresent(renderer);
}

void GameScene::exit() {
}

std::shared_ptr<Scene> GameScene::changeScene() {
    return nullptr;
}
