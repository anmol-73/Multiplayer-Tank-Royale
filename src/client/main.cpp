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
#endif

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED);
    InitWindow(1600, 900, "Multiplayer Tank Royal (Demo)");
    
    SetTargetFPS(120);

    Global::init();

    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::MAIN_PAGE, new Pages::MainWindowScene());
    SceneManagement::SceneManager::init();

    SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::MAIN_PAGE);   

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    Global::cleanup();
    return 0;
}