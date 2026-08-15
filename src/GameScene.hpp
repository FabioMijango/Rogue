#pragma once
#include <SDL3/SDL_stdinc.h>

#include "Bible.hpp"
#include "SGEL.hpp"
#include "SpatialGrid.hpp"
#include "utils/DungeonGenerator.hpp"

enum class NextScene {
    NextLevel,
    InitScene,
    None
};

class GameScene : public Scene {
    EntityManager entityManager;
    SpatialGrid spatialGrid = {bb::TILE_SIZE * 2.f, &entityManager}; // TODO: Update SGEL Version, and remove SpatialGrid include
    NextScene nextSceneType = NextScene::None;

    Entity player = -1;
    Dungeon dungeon;

    std::vector<Entity> entitiesToDestroy = {};

    void renderTiles(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderStair(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderEnemies(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderPlayer(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderCollisionBoxes(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderUI(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);

    void destroyExpiredEntities(Uint64 now);
public:

    bool init(void** screenData) override;
    void exit() override;

    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer *renderer) override;

    std::shared_ptr<Scene> changeScene() override;

};
