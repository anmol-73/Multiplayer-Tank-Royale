#ifndef HEADER_COMMON_STRUCTS
#define HEADER_COMMON_STRUCTS

#include <vector>
#include <raylib.h>

namespace CommonStructs
{
    struct PlayerPacket
    {
        float last_shot;
        bool is_idle;
        int ID;
        Vector2 position_absolute; // Top left of tank
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

    struct Projectile
    {
        Rectangle hitbox;
        double angle;
        double time_alive;
        double projectile_speed;
    };

}

#endif