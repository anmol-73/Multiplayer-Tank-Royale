#include "scene_management.hpp"
#include "scenes/game_window/game_window_scene.hpp"
#include "scenes/main_window/main_window_scene.hpp"
#include "scenes/new_game_window/new_game_window_scene.hpp"

#include "iostream"

void SceneManagement::init()
{
    register_scene(SceneManagement::SceneName::GAME_WINDOW, new Scenes::GameWindowScene());
    register_scene(SceneManagement::SceneName::MAIN_WINDOW, new Scenes::MainWindowScene());
    register_scene(SceneManagement::SceneName::NEW_GAME_WINDOW, new Scenes::NewGameWindowScene());
    should_run = true;

    for (unsigned long long int i = 0; i < scenes.size(); ++i){
        if (scenes[i].get() == nullptr){
            throw std::runtime_error("ERROR! Unregistered scene");
        }
    }
}

void SceneManagement::quit()
{
    should_run = false;
    _unload_scene();
}

void SceneManagement::join()
{
    thread_pool.stop(true);
}

Scene *SceneManagement::get_active_scene()
{
    if (loaded_scene != SceneName::__SCENE_NAME_COUNT__){
        return scenes[loaded_scene].get();
    }
    return nullptr;
}

void SceneManagement::register_scene(SceneName name, Scene *scene)
{
    if (name == SceneName::__SCENE_NAME_COUNT__){
        throw std::runtime_error("Invalid scene to register!");
    }
    if (scenes[name].get() != nullptr){
        throw std::runtime_error("Scene has already been registered!");
    }
    scenes[name].reset(scene);
}

void SceneManagement::load_scene(SceneName scene)
{
    if (scene == SceneName::__SCENE_NAME_COUNT__){
        throw std::runtime_error("Invalid scene to load!");
    }

    _unload_scene();
    loaded_scene = scene;
    thread_pool.push(
        [scene](int){
            scenes[scene].get()->_load();
        }
    );
}

void SceneManagement::_unload_scene()
{
    SceneName scene = loaded_scene;
    loaded_scene = SceneName::__SCENE_NAME_COUNT__;
    if (scene == SceneName::__SCENE_NAME_COUNT__) return;

    scenes[scene].get()->load_done = false;
    if(scenes[scene].get()->load_with_context_called==true) 
    {
        scenes[scene].get()->unload_with_context();
        scenes[scene].get()->load_with_context_called = false;
    }
    thread_pool.push(
        [scene](int){
            scenes[scene].get()->_unload();
        }
    );
}
