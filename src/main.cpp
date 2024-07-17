#include <iostream>

#include "core/global.hpp"
#include "core/global.cpp" // init requires Scamlib (and Scamlib requries Global lmao)
// Ideally you would create another header + cpp file but fuck that

#include "core/scene_management/scene_management.hpp"


int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "git gud vro dhruv");
    SetWindowMinSize(800, 450);
    SetTargetFPS(60);  

    Global::init();
    
    SceneManagement::init();
    SceneManagement::load_scene(SceneManagement::SceneName::MAIN_WINDOW);

    // NOTE! Scene management is not threadsafe, once should only use its functions from the main thread
    while (SceneManagement::should_run){
        Scene* active_scene = SceneManagement::get_active_scene();
        
        if (active_scene == nullptr){
            // If its ever null, somebody fucked up
            std::cerr << "ERROR! There is no active scene" << std::endl;
            break;
        }
        
        active_scene->_update();
        
    }

    Global::free();
    SceneManagement::join();
    return 0;
}
