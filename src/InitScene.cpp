#include "InitScene.hpp"
#include "Bible.hpp"
#include "GameScene.hpp"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"

bool InitScene::init(void** screenData) {
    registerAction(SDL_BUTTON_LEFT, bb::Acts::LEFT_CLICK);
    registerAction(SDL_SCANCODE_SPACE, bb::Acts::SPACE);

    return true;
}

SDL_AppResult InitScene::update(float deltaTime) {
    return SDL_APP_CONTINUE;
}

SDL_AppResult InitScene::eventHandler(const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }
    return SDL_APP_CONTINUE;
}

void InitScene::sDoAction(const Action &action) {
    if (action.state == Action::State::Pressed) {
        if (action.name == bb::Acts::SPACE) {
            m_hasEnded = true;
        }
    }
}

void InitScene::sRender(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 12, 19, 23, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    const char* text = "Press SPACE to start the game or ESC to exit.";
    SDL_RenderDebugText(renderer, bb::WIDTH / 2.f - 90.f, bb::HEIGHT / 2.f, text);
    
    SDL_RenderPresent(renderer);
}

void InitScene::exit() {

}

std::shared_ptr<Scene> InitScene::changeScene() {
    return std::make_shared<GameScene>();
}
