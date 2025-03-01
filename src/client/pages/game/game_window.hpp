#ifndef H_PAGES_GAME_WINDOW
#define H_PAGES_GAME_WINDOW

#include <raylib.h>
#include <vector>

#include "core/scene_management.hpp"
#include "game.hpp"
#include "misc/task.hpp"
#include "core/dragonlib.hpp"
#include "game_client.hpp"

namespace Pages
{
    class GameWindowScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        /** Expects a pointer to an Address! */
        virtual void _prepare(const void *address, size_t command);
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        ServiceConsumers::GameClient *client = nullptr;

        Communication::Address address = {};

        Utils::Task client_worker;

        using PlayerName = char[32];
        struct PlayerDetail{
            PlayerName name;
            int id;
        };

        struct GameProps
        {
            PlayerDetail player_details[12];
            int map; // The index of the map used.
        }game_props;
        
        void game_update_callback(const Game::GameState* server_gs, size_t size);
        
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

        std::vector<Game::Frame> made_frames;

        size_t player_id;

        Game::GameState game_state;

        Game::Frame curr_frame;

    };
} // namespace Pages

#endif