#include "game_constants.hpp"

std::vector<Game::Types::TankType> Game::Data::tank_types = {
    {
        68.2, // Height
        50.6, // Width
        7, // Health
        120.0f, // Mov speed
        PI/2, // Rot speed
    },
    {
        76.8, // Height
        65.2, // Width
        11, // Health
        80.0f, // Mov speed
        PI/3, // Rot speed
    },
    {
        59.7, // Height
        42.6, // Width
        4, // Health
        150.0f, // Mov speed
        PI, // Rot speed
    },
};
std::vector<std::array<std::string, 3>> Game::Data::tank_resources = {
    {"resources/game_window/icons/BASIC_ICON.png", "Basic", "resources/game_window/tanks/BASIC.png"},
    {"resources/game_window/icons/HEAVY_ICON.png", "Heavy", "resources/game_window/tanks/HEAVY.png"},
    {"resources/game_window/icons/QUICK_ICON.png", "Quick", "resources/game_window/tanks/QUICK.png"}
};

std::vector<Game::Types::GunType> Game::Data::gun_types = {
    {
        160, // Width
        40, // Height
        1.7, // Rot speed
        1, // Dmg
        100000, // Range
        0.7, // Reload time
    }, // Gun
    {
        133, // Width
        33, // Height
        0.8, // Rot speed
        0, // Dmg
        0, // Range
        0.4, // Reload time
    }, // Cannon
    {
        160, // Height
        40, // Width
        0, // Rot speed
        0, // Dmg
        0, // Range
        2, // Reload time
    }, // Mortar
};

std::vector<std::array<std::string, 3>> Game::Data::gun_resources = {
    {"resources/game_window/icons/PISTOL_ICON.png", "Pistol", "resources/game_window/turrets/PISTOL.png"},
    {"resources/game_window/icons/ROCKET_ICON.png", "Rocket", "resources/game_window/turrets/ROCKET.png"},
    {"resources/game_window/icons/MORTAR_ICON.png", "Mortar", "resources/game_window/turrets/MORTAR.png"}
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
        100, // Radius
        5, // Dmg
        2000, // Lifetime
    },
};