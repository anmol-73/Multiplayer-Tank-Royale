#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>


namespace SceneManagement
{
    enum SceneName{
        MAIN_PAGE,
        __NIL__ // NOTE: This must be the last position in the enum
    };

    class Scene{
    public:
        /** 
         * Function called exactly once when the SceneManager is initialized.
         * Spawns the load-unload thread.
         * 
         * NOTE: If overloaded, ensure that Scene::init is also called!
         */
        virtual void _init();

        /** 
         * Function called exactly once when the SceneManager is asked to cleanup.
         * Joins the load-unload thread.
         * 
         * NOTE: If overloaded, ensure that Scene::destroy is also called!
         */
        virtual void _destroy();

        /**
         * Called every frame after the scene has been loaded.
         */
        virtual void _update(){};

        /**
         * Called every frame while the scene is still loading.
         */
        virtual void _loading_update(){};

        /** Pushes the load request onto the load-unload thread. */
        void __request_load();
        
        /** Pushes the cleanup request onto the load-unload thread. */
        void __request_cleanup();

        /** 
         * Called every frame.
         * NOTE: This checks whether the scene is loading and calls the required update function.
         */
        void __update();


    protected:
        /**
         * Function called when a scene is asked to load itself.
         * 
         * NOTE: It is assured that for every `load` a `cleanup` will be called before another load
         */
        virtual void _load(){};

        /**
         * Function called when a scene is asked to cleanup any resources loaded in `load`.
         * 
         * NOTE: It is not assured that cleanup_with_context is called before `cleanup`. (Refer to note in `load_with_context`)
         * NOTE: It is assured that an `load` was done before every time `cleanup` is called.
         */
        virtual void _cleanup(){};
        
        /**
         * Function called when a scene is asked to load itself.
         * 
         * NOTE: It is not assured to be called after a load!!! If a scene was asked to be unloaded before it finished `load`, `load_with_context` is not called.
         * NOTE: This is called on the main thread after every `load`.
         * NOTE: It is assured that for every `load_with_context` a `cleanup_with_context` will be called
         */
        virtual void _load_with_context(){};
        
        /**
         * Function called when a scene is asked to cleanup any resources loaded in `load_with_context`.
         * 
         * NOTE: This is called on the main thread before `cleanup` is called.
         * NOTE: It is assured that an `load_with_context` was done before every time `cleanup_with_context` is called.
         */
        virtual void _cleanup_with_context(){};

        /**
         * Tells whether the load requested was cancelled (By a cleanup request being made)
         */
        inline bool load_is_cancelled();
        

    private:
        struct LoadParameters{
            std::mutex load_unload_mutex; // Protects `loads_requested` and `unloads_requested`
            std::condition_variable load_unload_cv;
            
            bool thread_should_run = false; // Flag that tells whether the load_unload thread should run
            std::thread load_unload_worker_thread; // The thread that handles all load and unload calls for the scene

            size_t loads_requested = 0; // Keeps track of the number of loads requested
            size_t unloads_requested = 0; // Keeps track of the number of unloads requested

            bool has_loaded_without_context = false; // Updated only by the load_unload_worker_thread
            bool has_loaded_with_context = false; // Updated only in the main thread            
        } load_parameters;

        /**
         * Run on another thread, handles running of load and cleanup functions
         */
        void load_unload_worker();
        
        /**
         * Tells whether the scene is currently active (i.e It is successfully loaded)
         */
        inline bool is_active();
    };
} // namespace SceneManagement


#include "scene.cpp"
