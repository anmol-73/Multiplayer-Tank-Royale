#ifndef HEADER_SERVER_LOGIC
#define HEADER_SERVER_LOGIC

#include "maps/maps.hpp"
#include "collisions.hpp"

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

struct PlayerPacket
{
    int ID;
    Vector2 position_absolute_units; // Top left of tank
    float player_angle; // Angle with x axis from tank center in rads
    float gun_angle; // Angle with x axis from tank center in rads
    int health=0;
    int gun_dmg;
    bool has_shot;
    bool is_alive;
    bool is_connected;
};

class GameState
{
    private:
        int max_players=8;

        std::vector<PlayerPacket> old_state;

        void init_state(int max_players);

        void update_state(PlayerPacket received_packet);

        void handle_tank_collision(PlayerPacket received_packet);

        struct GameConstants
        {
            std::vector<Vector2> spawnpoints = {
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
            }; // 8 Spawns in MTR_Units

            float player_height = 120; // MTR_Units
            float player_width = 160; // MTR_Units
            float wall_width = 16; // MTR_Units
        } game_constants;
};

#endif


/***
 * Making "MTR_Units"
 * 1 screen always 1920 units by 1080 units
 * 1 tile always 16 units by 16 units
 * So 1 screen always 120 tiles by 67.5 tiles
 * Tank always 160 units by 120 units
 * Map is 480 tiles by 270 tiles i.e. 7680 units by 4320 units
 * Clients must send all the data in "MTR_Units"
 * Clients must scale to render according to their screen size and aspect ratio
 ***/