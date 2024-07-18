#pragma once

#include <stdexcept>
#include <mutex>
#include <condition_variable>

class Scene{
public:
    /**
     * Called when the scene should load itself
     * NOTE: This is run on another thread, update calls will still be dispatched to the scene (Handle accordingly)
     */
    virtual void _load(){
        std::unique_lock<std::mutex> lock(load_unlock_mutex);
        load_ready_cv.wait(lock, [this](){return (this->load_ready);});
        load_ready = false;

        load_without_context();

        load_done = true;
        context_load_now = true;
        unload_ready_cv.notify_one();
    }

    /**
     * The function to overload if the scene requires to load stuff
     * Only the stuff that can run on non main thread should go here
     * For eg: Texture2D loading requires openGL context so it has to be done in main thread
     */
    virtual void load_without_context(){}

    /**
     * The function to overload if the scene requires to load stuff that has to be in main thread
     */
    virtual void load_with_context(){}

    /**
     * Called the the scene should unlod itself
     * NOTE: The scene will not recieve anymore update calls (Until load is called again)
     * NOTE: It is assured that an unload call will be made before the application quits
     * NOTE: You must do everythign done here vro
     */
    virtual void _unload(){
        std::unique_lock<std::mutex> lock(load_unlock_mutex);
        unload_ready_cv.wait(lock, [this](){return !(this->load_ready);});
        load_ready = true;
        
        unload_without_context();

        load_ready_cv.notify_one();
    }

    /**
     * The functino to overload if the scene requires to unload stuff anywhere
     */
    virtual void unload_without_context(){}

    /**
     * The functino to overload if the scene requires to unload stuff in main thread
     */
    virtual void unload_with_context(){}


    /**
     * Called every frame for the scene to handle logic updates and do draw calls (Called in the main thread)
     * NOTE: The scene must handle quit events
     * NOTE: The scene will receive update events while the load function is running
     */
    virtual void update(){
        throw std::runtime_error("All scenes must overload the update function!");
    }

    virtual ~Scene(){};
    /**
     * A flag that tells whether the scene is currently loading or not
     */
    bool load_done = false;

    /**
     * A flag that tells whether load_with_context should be run
     */
    bool context_load_now = false;

    /**
     * A flag that tells whether load_with_context was called
     */
    bool load_with_context_called = false;

    /**
     * Calls update, makes sure load_with_context runs once
     * 
     */
    virtual void _update()
    {
        if (context_load_now && load_done)
        {
            load_with_context();
            context_load_now = false;
            load_with_context_called = true;
        }
        update();
    }

protected:
    /**
     * Mutex that is locked when loading or unloading takes place
     */
    std::mutex load_unlock_mutex;
    std::condition_variable load_ready_cv;
    std::condition_variable unload_ready_cv;

    bool load_ready = true;
};