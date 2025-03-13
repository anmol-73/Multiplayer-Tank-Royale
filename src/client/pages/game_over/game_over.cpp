#include "game_over.hpp"
#include <iostream>

void Pages::GameOverScene::_update()
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

    if(ui.go_to_lobby->clicked){
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
    }
}

void Pages::GameOverScene::_loading_update()
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

void Pages::GameOverScene::_prepare(const void *msg, size_t command)
{
    switch(command){
        case 0: {
            assert(msg != nullptr);
            pi.list_size = *static_cast<const size_t*>(msg);
            break;
        }
        case 1: {
            assert(msg != nullptr);
            auto ptr = static_cast<const std::pair<std::string, int>*>(msg);

            ui.scores.assign(ptr, ptr + pi.list_size);
            for (const auto [n, s]: ui.scores){
                std::cout << n << " " << s << std::endl;
            }
            break;
        }

        default: assert(false);
    }
}

void Pages::GameOverScene::_load()
{
    ui.load_async();
}

void Pages::GameOverScene::_cleanup()
{
    ui.cleanup_async();
}

void Pages::GameOverScene::_load_with_context()
{
    ui.load_sync();
}

void Pages::GameOverScene::_cleanup_with_context()
{
    ui.cleanup_sync();
}
