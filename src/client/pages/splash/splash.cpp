#include "splash.hpp"

void Pages::SplashScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
        ui.draw();
    }
    EndDrawing();
    ui.poll_events();
}

void Pages::SplashScene::_loading_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0xcb, 0xc6, 0xb2, 0xe0});
        DragonLib::Utils::Drawing::draw_text({
            .content = "Loading...",
            .font_size = Global::rem * 2,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::SplashScene::_load()
{
    ui.load_async();
}

void Pages::SplashScene::_cleanup()
{
    ui.cleanup_async();
}

void Pages::SplashScene::_load_with_context()
{
    ui.load_sync();
}

void Pages::SplashScene::_cleanup_with_context()
{
    ui.cleanup_sync();
}
