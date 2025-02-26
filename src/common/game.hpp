#ifndef H_COM_GAME
#define H_COM_GAME

#include <raylib.h>
#include <stdlib.h>
#include <vector>
#include <raymath.h>
#include <chrono>

#include "game_constants.hpp"
#include "maps/maps.hpp"
#include "physics/collisions.hpp"

namespace Game
{
    // Data sent by client to server
    struct Frame
    {
        size_t player_id;

        size_t frame_num;

        Vector2 mouse_position_screen;
        
        bool w_pressed;
        
        bool a_pressed;
        
        bool s_pressed;
        
        bool d_pressed;
        
        bool lmb_pressed;

        double delta_time;
    };

    // Current state of the game
    struct GameState
    {
        size_t num_of_players;

        size_t map_num;

        long double time_of_last_objects_update;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

        struct PlayerState
        {
            bool is_alive;

            size_t last_frame_processed_num;

            size_t tank_type;
            
            size_t gun_type;

            Vector2 position;

            double angle;

            double gun_angle;

            double time_since_last_shot;

            int health;

            int score;

            Vector2 ray_contact;
        };

        std::vector<Game::GameState::PlayerState> player_vector;

        struct Projectile
        {
            size_t type;

            Vector2 position;

            double angle;

            double time_alive;

            size_t shot_id;
        };
        
        std::vector<Game::GameState::Projectile> projectile_vector;

        struct Explosion
        {
            size_t type;

            Vector2 position;

            double time_alive;

            size_t shot_id;
        };
        
        std::vector<Game::GameState::Explosion> explosion_vector;

        // Current frame being processed, so that it doesnt need to be passed around to functions
        Frame curr_frame;

        /**
         * Takes a frame as input and updates game state accordingly
         */
        void apply_frame(const Game::Frame& frame);

        /**
         * Keeps angle between 0 and 2pi
         */
        double normalize_angle(double angle);

        /**
         * To update player_position based on button presses after delta_time
         * To handle tank collision with walls and other tanks
         */ 
        void handle_movement();

        /**
         * To update angle to after delta_time
         */
        void set_gun_angle();

        /**
         * To handle shooting, reloading behaviour
         */
        void handle_shots();

        /**
         * To move projectiles and collide them
         */
        void update_projectiles(float delta_time);

        /**
         * To eventually cause explosions
         */
        void update_explosions(float delta_time);
    
        long double curtime();

    };

}

#endif