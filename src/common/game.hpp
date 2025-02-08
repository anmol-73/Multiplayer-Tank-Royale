#ifndef H_COM_GAME
#define H_COM_GAME

#include <raylib.h>
#include <stdlib.h>
#include <vector>
#include <raymath.h>
#include "game_constants.hpp"
#include "maps/maps.hpp"
#include "physics/collisions.hpp"

namespace Game
{
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

    struct GameState
    {
        size_t num_of_players;

        size_t map_num;

        struct PlayerState
        {
            bool is_alive;

            size_t last_frame_processed_num;

            size_t tank_type;
            
            size_t gun_type;

            Vector2 position;

            double angle;

            double gun_angle;

            size_t frame_on_last_shot;
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

        Frame curr_frame;

        void apply_frame(const Game::Frame& frame);

        double normalize_angle(double angle);

        /**
         * To update player_position based on button presses after delta_time
         */ 
        void handle_movement();

        /**
         * To update angle to after delta_time
         */
        void set_gun_angle();
        };
    }

#endif