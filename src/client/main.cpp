#include <raylib.h>

#include "core/scene_management/scene_manager.hpp"
#include "pages/pages.hpp"

#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
    #undef ABSOLUTE
    #undef min
    #undef max
#endif

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    
    InitWindow(800, 450, "Multiplayer Tank Royal (Demo)");
    size_t window_width = std::min(GetMonitorWidth(GetCurrentMonitor()) - 200, 1600);
    SetWindowSize(window_width, (window_width * 9) / 16);
    SetWindowPosition(0, 0);
    SetTargetFPS(120);

    Global::init();

    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::MAIN_PAGE, new Pages::MainWindowScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::LOBBY_PAGE, new Pages::LobbyWindowScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::GAME_PAGE, new Pages::GameWindowScene());
    SceneManagement::SceneManager::init();

    SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::MAIN_PAGE);   

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    Global::cleanup();
    return 0;
}