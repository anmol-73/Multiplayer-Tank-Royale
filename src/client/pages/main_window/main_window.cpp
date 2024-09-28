#include "main_window.hpp"


void Pages::MainWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
           
    }
    EndDrawing();
}

void Pages::MainWindowScene::_loading_update() { return _update(); }