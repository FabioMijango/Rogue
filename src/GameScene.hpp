#pragma once
#include "SGEL.hpp"
#include "SpatialGrid.hpp"
#include "utils/DungeonGenerator.hpp"

class GameScene : public Scene {
    EntityManager entityManager;
    SpatialGrid spatialGrid = {bb::ROOM_SIZE, &entityManager}; // TODO: Update SGEL Version, and remove SpatialGrid include

    Entity player = -1;
    Dungeon dungeon;

    std::vector<Entity> entitiesToDestroy = {};

    void renderTiles(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderEnemies(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderPlayer(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);
    void renderCollisionBoxes(SDL_Renderer *renderer, const CameraComponent *camera, const SDL_FPoint &screenSize);

    // TODO: Add specific system for destroying entities
    // void destroyExpiredEntities(Uint64 now);
public:

    bool init(void** screenData) override;
    void exit() override;

    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer *renderer) override;

    std::shared_ptr<Scene> changeScene() override;

};
