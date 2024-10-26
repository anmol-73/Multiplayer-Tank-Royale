#ifndef HEADER_SERVER_LOGIC
#define HEADER_SERVER_LOGIC
#include <raylib.h>

#include "maps/maps.hpp"
#include "collisions.hpp"

#include <string>
#include <utility>
#include <vector>

struct PlayerPacket
{
    float last_shot;
    bool is_idle;
    int ID;
    Vector2 position_absolute_units; // Top left of tank
    float player_angle; // Angle with x axis from tank center in rads
    float gun_angle; // Angle with x axis from tank center in rads
    int health;
    int player_dmg;
    bool has_shot;
    bool is_alive;
    bool is_connected;
    Vector2 closest_wall_hit;
    int score;
};

class GameState
{
    public:
        int max_players=8;

        std::vector<PlayerPacket> old_state;

        void init_state(int max_players);

        std::vector<PlayerPacket>& update_state(PlayerPacket* received_packet);

        void handle_tank_collision(PlayerPacket* received_packet);

        void handle_shots(PlayerPacket* received_packet);

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