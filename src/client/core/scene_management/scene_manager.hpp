#pragma once

#include "scene.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

namespace SceneManagement
{
    namespace SceneManager
    {
        /** 
         * A flag telling is the SceneMangaer is active.
         * NOTE: The main thread uses this to know whether to keep running or not.
         */
        bool is_active = false;

        /** An enum containing the name of the currently loaded scene. */
        SceneName _loaded_scene = SceneName::__NIL__;

        /** A vector keeping reference of all the scenes in the application. */
        std::vector<std::unique_ptr<Scene>> _scenes(SceneName::__NIL__);

        /**
         * Initializes the scene manager.
         * 
         * NOTE: All scenes must have been registered before this method is called!
         */
        void init();

        /**
         * Cleans up all resources associated with the SceneManager.
         */
        void cleanup();

        /**
         * Updates the active scene.
         */
        void update();

        /**
         * Registers a new scene to the scene manager.
         */
        void register_scene(SceneName scene_name, Scene* scene);

        /**
         * Unloads the currently loaded scene and loads the new scene.
         * 
         * NOTE: This function must only be called from the main thread!
         * NOTE: If `ignore_reload` is set to true and new sene is the currently loaded scene, nothing is done.
         */
        void load_scene(SceneName scene, bool ignore_reload=true);
        
        /**
         * Unloads the currently loaded scene.
         */
        void _unload_loaded_scene();

        /**
         * Unloads the current scene and deactivates the SceneManager.
         * 
         * NOTE: Does nothing if the SceneManager is already inactive.
         */
        void quit();
    } // namespace SceneManager
    
} // namespace SceneManagement
