#ifndef H_PAGES_GAME_WINDOW
#define H_PAGES_GAME_WINDOW

#include <raylib.h>
#include <vector>
#include <cassert>

#include "core/scene_management.hpp"
#include "game.hpp"
#include "misc/task.hpp"
#include "core/dragonlib.hpp"
#include "game_client.hpp"
#include "game_ui.hpp"
#include "core/utils/animation_controller.hpp"
#include "core/utils/camera.hpp"


namespace Pages
{
    class GameWindowScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        /** Expects a pointer to an Address! */
        virtual void _prepare(const void *msg, size_t command);
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        GameUI ui;

        ServiceConsumers::GameClient *client = nullptr;        
        Utils::Task client_worker;
                
        struct PreparedArguments{
            Communication::Address address = {};
            
            size_t player_details_size = 0;
            std::vector<Communication::Game::PlayerIdentification> player_details;
            Communication::Room::RoomSettings settings; // The index of the map used.
            
            Communication::Game::PlayerIdentification pi;
        } prepared_args;
        
        std::vector<Game::Frame> made_frames;
        Game::Frame curr_frame;
        
        Game::GameState game_state;        
        
        void game_update_callback(const Game::GameState server_gs, size_t size);
        
        /**
         * Handles updating game logic each frame
         * 
         * RETURNS: true if the drawing should be skipped this frame (For example if scene is changed)
         */
        void logic_update();

        /**
         * Handles setting current frame based on player inputs
         */
        void set_curr_frame();
        
    };
} // namespace Pages

#endif