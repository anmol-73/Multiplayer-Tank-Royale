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
        std::vector<Game::Types::TankType> tank_types = {
            {
                64, // Height
                48, // Width
                5, // Health
                120.0f, // Mov speed
                PI/2, // Rot speed
            },
        };

        std::vector<Game::Types::GunType> gun_types = {
            {
                48, // Height
                12, // Width
                1, // Dmg
                1, // Rot speed
                100000, // Range
                1, // Reload time
            }, // Gun
            {
                48, // Height
                12, // Width
                0, // Dmg
                1, // Rot speed
                0, // Range
                1, // Reload time
            }, // Cannon
            {
                48, // Height
                12, // Width
                0, // Dmg
                0, // Rot speed
                0, // Range
                2, // Reload time
            }, // Mortar
        };

        std::vector<Game::Types::ProjectileType> projectile_types = {
            {
                12, // Height
                12, // Width
                200.0f, // Mov Speed
                1, // Dmg
                5, // lifetime
            },
        };

        std::vector<Game::Types::ExplosionType> explosion_types = {
            {
                20, // Radius
                3, // Dmg
                2, // Lifetime
            },
        };
    }
}

#endif