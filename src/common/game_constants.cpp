#include "game_constants.hpp"

std::vector<Game::Types::TankType> Game::Data::tank_types = {
    {
        64, // Height
        48, // Width
        5, // Health
        120.0f, // Mov speed
        PI/2, // Rot speed
    },
};
extern std::vector<std::pair<std::string, std::string>> Game::Data::tank_resources = {
    {"resources/game_window/tank2_spritesheet.png", "Basic"}
};

std::vector<Game::Types::GunType> Game::Data::gun_types = {
    {
        48, // Width
        12, // Height
        1, // Rot speed
        1, // Dmg
        100000, // Range
        1, // Reload time
    }, // Gun
    {
        48, // Width
        12, // Height
        1, // Rot speed
        0, // Dmg
        0, // Range
        1, // Reload time
    }, // Cannon
    {
        48, // Height
        12, // Width
        0, // Rot speed
        0, // Dmg
        0, // Range
        2, // Reload time
    }, // Mortar
};

extern std::vector<std::pair<std::string, std::string>> Game::Data::gun_resources = {
    {"resources/game_window/tank2_spritesheet.png", "Pistol"},
    {"resources/game_window/tank2_spritesheet.png", "Rocket"},
    {"resources/game_window/tank2_spritesheet.png", "Mortar"}
};

std::vector<Game::Types::ProjectileType> Game::Data::projectile_types = {
    {
        12, // Height
        12, // Width
        0.2, // Mov Speed
        1, // Dmg
        5000, // lifetime
    },
};

std::vector<Game::Types::ExplosionType> Game::Data::explosion_types = {
    {
        20, // Radius
        3, // Dmg
        2000, // Lifetime
    },
};