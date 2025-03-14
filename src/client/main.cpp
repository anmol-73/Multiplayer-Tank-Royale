#include <raylib.h>
#include <enet_include.hpp>
#include <stdexcept>
#include <iostream>

#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "pages.hpp"

int main(int argc, char* argv[]){
    if (enet_initialize()){
        throw std::runtime_error("Could not initialize enet!");
    }

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "Multiplayer Tank Royal (Demo)");{
        size_t screen_height = GetMonitorHeight(GetCurrentMonitor());
        size_t screen_width = GetMonitorWidth(GetCurrentMonitor());
        
        size_t window_width = std::min(GetMonitorWidth(GetCurrentMonitor()) - 200, 1600);
        size_t window_height = window_width * 9 / 16;
        
        SetWindowSize(window_width, window_height);
        SetWindowPosition((screen_width - window_width) / 2, (screen_height - window_height) / 2);
    }
    SetTargetFPS(60);
    
    Global::init();

    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::SPLASH, new Pages::SplashScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::LOBBY, new Pages::LobbyScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::ROOM, new Pages::RoomScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::GAME, new Pages::GameWindowScene());
    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::GOVER, new Pages::GameOverScene());
    SceneManagement::SceneManager::init();

    SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH);

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    Global::cleanup();

    return 0;
}