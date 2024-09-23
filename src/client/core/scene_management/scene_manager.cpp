#include "scene_manager.hpp"

bool SceneManagement::SceneManager::is_active(true);
SceneManagement::SceneName SceneManagement::SceneManager::_loaded_scene(__NIL__);
std::vector<std::unique_ptr<SceneManagement::Scene>>SceneManagement::SceneManager::_scenes(__NIL__);

void SceneManagement::SceneManager::init()
{
    // Just a sanity check to ensure that all defined scenes have been registered
    for (const auto &scene: _scenes){
        if (scene.get() == nullptr){
            throw std::runtime_error("ERROR! Unregistered scene");
        }
    }

    for (const auto &scene: _scenes){
        scene->_init();
    }
}

void SceneManagement::SceneManager::cleanup()
{
    quit();
    
    // Clean up all the scenes
    for (const auto &scene: _scenes){
        scene->_destroy();
    }
}

void SceneManagement::SceneManager::update()
{
    if (_loaded_scene == SceneName::__NIL__) return;
    _scenes[_loaded_scene].get()->__update();
}

void SceneManagement::SceneManager::register_scene(SceneName scene_name, Scene *scene)
{
    if (scene_name == SceneName::__NIL__){
        throw std::runtime_error("Invalid scene to register!");
    }
    if (_scenes[scene_name].get() != nullptr){
        throw std::runtime_error("Scene has already been registered!");
    }
    _scenes[scene_name].reset(scene);
}

void SceneManagement::SceneManager::load_scene(SceneName scene, bool ignore_reload)
{
    _unload_loaded_scene();
    _loaded_scene = scene;
    _scenes[_loaded_scene].get()->__request_load();
}

void SceneManagement::SceneManager::_unload_loaded_scene()
{
    if (_loaded_scene == SceneName::__NIL__) return;
    _scenes[_loaded_scene].get()->__request_cleanup();
    _loaded_scene = SceneName::__NIL__;
}

void SceneManagement::SceneManager::quit()
{
    if (!is_active) return;
    is_active = false;
    _unload_loaded_scene();
}
