#include "game_constants.hpp"

std::vector<Game::Types::TankType> Game::Data::tank_types = {
    {
        64, // Height
        48, // Width
        5, // Health
        120.0f, // Mov speed
        PI/2, // Rot speed
    },
    {
        72, // Height
        56, // Width
        7, // Health
        80.0f, // Mov speed
        PI/3, // Rot speed
    },
    {
        56, // Height
        40, // Width
        4, // Health
        150.0f, // Mov speed
        PI, // Rot speed
    },
};
std::vector<std::array<std::string, 3>> Game::Data::tank_resources = {
    {"resources/game_window/tank2_spritesheet.png", "Basic", "resources/game_window/tanks/BASIC.png"},
    {"resources/game_window/tank2_spritesheet.png", "Heavy", "resources/game_window/tanks/HEAVY.png"},
    {"resources/game_window/tank2_spritesheet.png", "Quick", "resources/game_window/tanks/QUICK.png"}
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

std::vector<std::array<std::string, 3>> Game::Data::gun_resources = {
    {"resources/game_window/tank2_spritesheet.png", "Pistol", "resources/game_window/tank2_spritesheet.png"},
    {"resources/game_window/tank2_spritesheet.png", "Rocket", "resources/game_window/tank2_spritesheet.png"},
    {"resources/game_window/tank2_spritesheet.png", "Mortar", "resources/game_window/tank2_spritesheet.png"}
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