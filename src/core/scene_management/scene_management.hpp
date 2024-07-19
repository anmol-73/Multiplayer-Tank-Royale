#pragma once

#include <vector>
#include <memory>
#include <stdexcept>

#include "scene.hpp"
#include <ctpl_stl.hpp>

namespace SceneManagement
{
    ctpl::thread_pool thread_pool(4);

    enum SceneName{
        MAIN_WINDOW,
        game_window,
        __SCENE_NAME_COUNT__
    };

    /**
     * Flag that tells whether the application should continue executing or not
     */
    bool should_run = false;

    /**
     * Registers all scenes
     * Throws an exception if all scenes in the enum SceneName have not been registered
     */
    void init();

    /**
     * Tells the mainloop to stop running and unloads the active scene
     */
    void quit();

    /**
     * Basically waits until all loads and unloads succesfully complete
     */
    void join();

    /**
     * Returns the raw ptr to the loaded scene if it exists, else returns nullptr
     */
    Scene* get_active_scene();

    /**
     * Stores the name of the currently loaded scene
     * NOTE: Set to `__SCENE_NAME_COUNT__` (which is an invalid scene name) if no scene is currently loaded
     */
    SceneName loaded_scene = SceneName::__SCENE_NAME_COUNT__;

    /**
     * A vector containing all the scenes lmao
     */
    std::vector<std::unique_ptr<Scene>> scenes(SceneName::__SCENE_NAME_COUNT__);

    /**
     * Registers a scene in the scene manager
     * RAISES: A runtime exception, if a registered scene is registered again
     */
    void register_scene(SceneName name, Scene* scene);

    /**
     * Unloads the currently loaded scene (if it exists) and then loads the given scene
     * RAISES: A runtime exception, if an invalid scene is asked to be loaded
     */
    void load_scene(SceneName scene);

    /**
     * Unloads the currently loaded scene (if it exists)
     */
    void _unload_scene();
} // namespace SceneManagement

#include "scene_management.cpp"
