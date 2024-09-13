#include "core/scene_management/scene_manager.hpp"


int main(int argc, char const *argv[])
{
    SceneManagement::SceneManager::init();    

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    return 0;
}
