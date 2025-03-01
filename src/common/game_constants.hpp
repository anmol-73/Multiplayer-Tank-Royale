#ifndef H_COM_GAME_CONSTANTS
#define H_COM_GAME_CONSTANTS

#include <raylib.h>
#include <stdlib.h>
#include <vector>

namespace Game
{
    namespace Types
    {
        struct TankType
        {
            double width;

            double height;

            int max_health;

            double move_speed;

            double rot_speed;
        };

        struct GunType
        {
            double width;

            double height;

            double rot_speed;

            int dmg;

            double range;

            double reload_time;
        };

        struct ProjectileType
        {
            double width;

            double height;

            double mov_speed;

            int dmg;

            double lifetime;
        };

        struct ExplosionType
        {
            double radius;

            int dmg;

            double lifetime;
        };
    }

    namespace Data
    {
        extern std::vector<Game::Types::TankType> tank_types;

        extern std::vector<Game::Types::GunType> gun_types;

        extern std::vector<Game::Types::ProjectileType> projectile_types;

        extern std::vector<Game::Types::ExplosionType> explosion_types;
    }
}

#endif