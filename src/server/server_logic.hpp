#ifndef HEADER_SERVER_LOGIC
#define HEADER_SERVER_LOGIC
#include <raylib.h>

#include "maps/maps.hpp"
#include "collisions.hpp"
#include "common_structs.hpp"

#include <string>
#include <utility>
#include <vector>
#include <chrono>


class GameState
{
    public:
        int max_players=8;

        std::vector<CommonStructs::PlayerPacket> old_state;

        long double time_of_last_proj_update;
        std::chrono::_V2::system_clock::time_point start_time;

        void init_state(int max_players);

        std::vector<CommonStructs::PlayerPacket>& update_state(CommonStructs::PlayerPacket* received_packet);

        void handle_tank_collision(CommonStructs::PlayerPacket* received_packet);

        void handle_shots(CommonStructs::PlayerPacket* received_packet);

        std::vector<CommonStructs::Explosion> explosions_vector;

        std::vector<CommonStructs::Explosion>& update_explosions();

        struct GameConstants
        {
            std::vector<Vector2> spawnpoints = {
                {448, 240},
                {40, 40},
                {1900, 1000},
                {150, 180},
                {1024, 600},
            };

            float player_height = 48; // MTR_Units
            float player_width = 64; // MTR_Units
            int gun_dmg = 1;
        } game_constants;

        long double curtime();
};

#endif


/***
 * Making "MTR_Units"
 * 1 screen always 1024 units by 576 units
 * 1 tile always 32 units by 32 units
 * So 1 screen always 32 tiles by 18 tiles
 * Tank always 128 units by 96 units
 * Map is 64 tiles by 36 tiles i.e. 2048 units by 1152 units
 * Clients must send all the data in "MTR_Units"
 * Clients must scale to render according to their screen size and aspect ratio
 ***/