#pragma once

#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
#endif

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <mutex>
#include <list>
#include <ctpl_stl.hpp>

#include "core/scene_management/scene.hpp"
#include "core/scene_management/scene_management.hpp"
#include "core/scamlib/textbox.hpp"
#include "core/scamlib/text.hpp"
#include "core/maths.hpp"
#include "core/utils/animation_controller.hpp"

namespace Scenes{
    ctpl::thread_pool thread_pool(1);
    class GameWindowScene: public Scene{
    public:
        enum GameState{
            PRE_GAME,
            GAME,
            POST_GAME
        };
        void update();
        void loading_update();
        void load_without_context();
        void load_with_context();
        void unload_without_context();
        void unload_with_context();
        void data_receiver(int);
    
    private:

        ENetHost* client;
        ENetAddress address;
        ENetEvent event;
        ENetPeer *peer;

        struct DataPacket
        {
            float x;
            float y;
            double player_angle;
            double gun_angle;
            bool has_shot;
        };

        const Color player_color = GOLD;
        const unsigned int player_radius = 20;
        
        const Color gun_color = BLACK;
        

        const Color enemy_color = PINK;
        const unsigned int enemy_radius = 10;

        const Color drop_color = ORANGE;
        const unsigned int drop_radius = 5;

        const float bullet_range = 2000;


        /**
         * Handles updating game logic each frame
         * 
         * RETURNS: true if the drawing should be skipped this frame (For example if scene is changed)
         */
        void logic_update();

        /**
         * Draws out the game
         */
        void draw_game();

        /**
         * Draws the hud
         */
        void draw_hud();

        GameState state;

        std::vector<DataPacket> player_list;

        /**
         * The speed at which enemies move towards the player
         */
        float enemy_speed;

        unsigned long long int kills;

        /**
         * Basically a delta added to all coordinates :)
         */
        Vector2 camera;

        /**
         * The player position
         */
        Vector2 player_position;

        int player_id = -1;
        /**
         * Offset 
         */
        Vector2 offset;

        /**
         * The mouse position
         */
        Vector2 mouse;

        /**
         * The player size, speed, angle and angular velocity
         */
        struct PlayerData
        {
            Rectangle player = {
                .width = 70,
                .height = 100,
            };
            float player_speed;
            double player_angle;
            double sin_player;
            double cos_player;
            double player_rot_speed = PI/2;
        } player_data;

        Utils::AnimationController* player_controller;
        size_t player_idle_idx;
        size_t player_moving_idx;

        Utils::AnimationController* gun_controller;
        size_t gun_idle_idx;
        size_t gun_shot_idx;

        Texture2D player_spritesheet;
        Image player_spritesheet_image;
        bool main_load_ready;

        /**
         * The gun size, angle, sincos, angular speed, state
         */
        struct GunData
        {
            Rectangle gun = {
                .width = 20,
                .height = 80
            };
            double gun_angle;
            double predicted_gun_angle;
            double sin_gun;
            double cos_gun;
            double gun_rot_speed = 1; // rads per sec
            bool has_shot = false;
        } gun_data;

        /**
         * The gun size, angle, sincos, angular speed, state
         */
        struct CrosshairData
        {
            double mouse_distance;
            Vector2 tracker_position;
            double tracker_distance;
            const float tracker_radius = 10;
            double tracker_radial_speed = 500;
            Color circle_color = WHITE; 
        } crosshair_data;

        /**
         * To set gun and tank position to player_position
         */
        void set_position();

        /**
         * To update player_position and play animations based on button presses after delta_time
         */
        void handle_movement(float delta_time);

        /**
         * To make angle between pi and -pi
         */
        double normalize_angle(double angle);

        /**
         * To update angle to after delta_time
         */
        void set_gun_angle(float delta_time);

        /**
         * To update tracker circle to after delta_time
         */
        void set_tracker(float delta_time);

        /**
         * The number of bullets left
         */
        unsigned int ammo;

        /**
         * Position of all enemies
         */
        std::vector<Vector2> enemies;

        /**
         * Position of all ammo drops
         */
        std::list<Vector2> drops;

        /**
         * Gets a random position to spawn an enemy at
         */
        Vector2 spawn_enemy();

        /**
         * Generates a random number for the amount of ammo to be dropped
         */
        unsigned int get_drop_amount();

        std::mutex load_unload_mutex;

        void init_server_connection();
        void end_server_connection();
        void send_data();

        float pregame_timer;
        struct UIState{
            bool play_again_button_is_hovered = false;
            bool go_home_button_is_hovered = false;
        } ui_state;
    };
}

#include "game_window_scene.cpp"
