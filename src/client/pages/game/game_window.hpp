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
        ServiceConsumers::GameClient *client = nullptr;

        
        Utils::Task client_worker;
        
        using PlayerName = char[32];
        struct PlayerDetail{
            PlayerName name;
            int id;
        };
        
        
        Communication::Address address = {};
        size_t player_details_size = 0;
        std::vector<PlayerDetail> player_details;
        Communication::Room::RoomSettings settings; // The index of the map used.
        
        
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

        std::vector<Game::Frame> made_frames;

        size_t player_id;

        Game::GameState game_state;

        Game::Frame curr_frame;

        struct PlayerInfo
        {
            std::string name;
            int score;
        };

        std::vector<PlayerInfo> leaderboard;

        struct CrosshairData
        {
            Vector2 mouse_position; // Relative
            double mouse_distance; // Relative
            Vector2 tracker_position; // Relative and wrt top left
            double tracker_distance; // Relative
            double tracker_radius;
            double tracker_radial_speed;
            Color circle_color = WHITE; 

            void init();
        }crosshair_data;

        /**
         * To update tracker circle to after delta_time
         */
        void set_tracker(float delta_time);

        Texture2D player_spritesheet;
        Image player_spritesheet_image;
        
        std::vector<Utils::AnimationController> player_controllers;
        size_t player_idle_idx;
        size_t player_moving_idx;

        std::vector<Utils::AnimationController> gun_controllers;
        size_t gun_idle_idx;
        size_t gun_shot_idx;

        int map_idx;

        Utils::Animation *player_idle;
        Utils::Animation *player_moving;
        Utils::Animation *gun_idle;
        Utils::Animation *gun_shot;

        Texture2D map;
        Image map_image;

        Utils::Camera camera;

    };
} // namespace Pages

#endif