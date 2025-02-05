#ifndef H_COM_GAME
#define H_COM_GAME

#include <raylib.h>
#include <stdlib.h>
#include <vector>

namespace Game
{
    struct GameState
    {
        size_t num_of_players;

        struct PlayerState
        {
            bool is_alive;

            size_t last_frame_processed_num;

            size_t tank_type;
            
            size_t gun_type;

            Vector2 position;

            double angle;

            double gun_angle;

            bool shot_on_last_frame_processed;
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

    };
}

#endif