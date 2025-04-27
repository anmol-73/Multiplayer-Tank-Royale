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

    if(ui.connect_button->clicked){
        Communication::Address address(ui.address_input->value);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, &address, 0);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::LOBBY, "", 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::LOBBY);
    }
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

void Pages::SplashScene::_prepare(const void *error_msg, size_t command)
{
    switch(command){
        case 0: {
            assert(error_msg != nullptr);
            ui.error_message = static_cast<const char*>(error_msg);
            break;
        }

        case 1: {
            ui.error_message = "";
            break;
        }

        default: assert(false);
    }
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
