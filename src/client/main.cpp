#include <raylib.h>

#include "core/scene_management/scene_manager.hpp"

#include "pages/pages.hpp"

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "collision testing");
    SetWindowMinSize(800, 450);
    SetTargetFPS(60);

    Global::init();

    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::MAIN_PAGE, new Pages::MainWindowScene());
    SceneManagement::SceneManager::init();

    SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::MAIN_PAGE, true);

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    return 0;
}