#include "main_window.hpp"

void Pages::MainWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({80, 80, 80, 255});
        
    }
    EndDrawing();
}

void Pages::MainWindowScene::_loading_update() { return _update(); }