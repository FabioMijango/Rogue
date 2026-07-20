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
    Animation* floorAnim =  assets.loadAnimation("floor", bb::tileRes.name, { 0 , 7 * bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!floorAnim) return false;

    // Temp wall texture
    Animation* wallAnim =  assets.loadAnimation("wall", bb::tileRes.name, { 0 , 0, bb::ASSETS_TILE_SIZE, bb::ASSETS_TILE_SIZE });
    if (!wallAnim) return false;

    dungeon = DungeonGenerator().generate();

    registerAction(SDL_SCANCODE_SPACE, "SPACE");

    return true;
}

SDL_AppResult GameScene::update(float deltaTime) {
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
    if (action.state == Action::State::Pressed) {
        if (action.name == "SPACE") {
            dungeon = DungeonGenerator().generate();
        }
    }
}

void GameScene::sRender(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    const Animation& playerAnim = Assets::Instance().getAnimation("player");
    const auto rect = playerAnim.getSprite().m_textureRect;
    SDL_RenderTexture(renderer, playerAnim.getTexture(), &rect, nullptr);

    for (auto& room : dungeon.m_rooms | std::ranges::views::values) {
        for (auto& cell : room.cells) {

            auto& anim = cell.animation;
            auto sprite = anim->getSprite();

            SDL_RenderTexture(renderer, anim->getTexture(), &sprite.m_textureRect, &cell.tileBounds);
        }
    }

    SDL_RenderPresent(renderer);
}

void GameScene::exit() {
}

std::shared_ptr<Scene> GameScene::changeScene() {
    return nullptr;
}
