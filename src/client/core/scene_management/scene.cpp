#include "scene.hpp"

void SceneManagement::Scene::_init()
{
    // Start the load unload worker thread :)
    load_parameters.thread_should_run = true;
    load_parameters.load_unload_worker_thread = std::jthread([this](){this->load_unload_worker();});
}

void SceneManagement::Scene::_destroy()
{
    // Stop the load unload worker thread :)
    load_parameters.thread_should_run = false;
    load_parameters.load_unload_cv.notify_one();

    load_parameters.load_unload_worker_thread.join();
}

void SceneManagement::Scene::__request_load()
{
    // NOTE: This method is always called from the main thread

    // Increment the loads_requested
    load_parameters.load_unload_mutex.lock();
    ++load_parameters.loads_requested;
    load_parameters.load_unload_mutex.unlock();
    
    // Wake up the load_unload_worker
    load_parameters.load_unload_cv.notify_one();
}

void SceneManagement::Scene::__request_cleanup()
{
    // NOTE: This method is always called from the main thread
    
    // Cleanup with context if required
    if (load_parameters.has_loaded_with_context){
        _cleanup_with_context();
        load_parameters.has_loaded_with_context = false;
    }

    // Increment the unloads requested
    load_parameters.load_unload_mutex.lock();
    ++load_parameters.unloads_requested;
    load_parameters.load_unload_mutex.unlock();
    
    // Wake up the load_unload_worker
    load_parameters.load_unload_cv.notify_one();
}

void SceneManagement::Scene::_prepare(const void *data, size_t command){}

void SceneManagement::Scene::__update()
{
    if (!is_active()){
        _loading_update(); return; // i.e we are still loading
    }
    // We have completed loading
    
    if (!load_parameters.has_loaded_with_context){
        _load_with_context(); // Load with context if required
        load_parameters.has_loaded_with_context = true;
    }
    _update();
}

inline bool SceneManagement::Scene::is_active(){
    // Just think about it a little bit (It makes sense full trust)
    // Although its not required to && with has_loaded_without_context, it just makes it easier to read
    return load_parameters.loads_requested == 0 && load_parameters.unloads_requested == 0 && load_parameters.has_loaded_without_context;
}

inline bool SceneManagement::Scene::load_is_cancelled()
{
    // i.e Someone has requested a cleanup
    return load_parameters.unloads_requested > 0;
}

void SceneManagement::Scene::load_unload_worker()
{
    // NOTE: It might not be required to || with has_loaded_without_context
    while (load_parameters.thread_should_run || load_parameters.has_loaded_without_context){
        std::unique_lock<std::mutex> lock(load_parameters.load_unload_mutex);
        
        // Sleep until a load or unload has been requested (or if the worker should be killed)
        load_parameters.load_unload_cv.wait(lock, [this](){
            return (
                (!load_parameters.thread_should_run) || 
                load_parameters.loads_requested > 0  ||
                load_parameters.unloads_requested > 0
            );
        });
        
        if (!load_parameters.has_loaded_without_context){
            // i.e The scene is currently inactive
            // If multiple load and unload requests were made, we can safely reduce the number of
            // tasks requested
            load_parameters.loads_requested -= load_parameters.unloads_requested;
            load_parameters.unloads_requested = 0;
        }
        bool needs_to_load = load_parameters.loads_requested > 0;
        lock.unlock(); // Let go of the mutex

        if ((!load_parameters.has_loaded_without_context) && (!needs_to_load)){
            // If scene is currently inactive and doesn't need to load...
            continue;
        }

        if (!load_parameters.has_loaded_without_context){
            _load();
            
            // Decrement the loads_requested
            load_parameters.load_unload_mutex.lock();
            --load_parameters.loads_requested;
            load_parameters.load_unload_mutex.unlock();
            
            if (!load_is_cancelled()){
                load_parameters.has_loaded_without_context = true;
                continue;
            }
            // i.e If an unload was requested midway,
            // We must cleanup now itslef
            // NOTE: Both loading and cleaning with context were skipped
        }

        // i.e The scene must be cleaned up

        _cleanup();

        // Decrement the unloads_requested
        load_parameters.load_unload_mutex.lock();
        --load_parameters.unloads_requested;
        load_parameters.load_unload_mutex.unlock();
        
        load_parameters.has_loaded_without_context = false;    
    }
}

SceneManagement::Scene::~Scene() = default;
void SceneManagement::Scene::_cleanup_with_context(){};
void SceneManagement::Scene::_load_with_context(){};
void SceneManagement::Scene::_loading_update(){};
void SceneManagement::Scene::_cleanup(){};
void SceneManagement::Scene::_update(){};
void SceneManagement::Scene::_load(){};
