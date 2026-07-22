#include "GameScene.hpp"
#include "SGEL.hpp"

int main() {
    Engine& engine = Engine::Instance();
    auto result = engine.init({bb::WIDTH, bb::HEIGHT}, 60, std::make_unique<GameScene>());
    if (result != SDL_APP_CONTINUE) {
        engine.sQuit();
        return -1;
    }

    bool isRunning = true;
    while (isRunning) {
        SDL_Event event = {0};

        while (SDL_PollEvent(&event)) {
            if (engine.sUserInput(&event) != SDL_APP_CONTINUE) isRunning = false;
        }

        if (engine.update() != SDL_APP_CONTINUE) isRunning = false;
        engine.sRender();
    }

    engine.sQuit();

    return 0;
}
