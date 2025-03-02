#include "game_logic_helpers.hpp"
#include <iostream>

GameHelpers::CrosshairData GameHelpers::crosshair_data;
int GameHelpers::map_idx{};

Utils::Animation* GameHelpers::player_idle;
Utils::Animation* GameHelpers::player_moving;
Utils::Animation* GameHelpers::gun_idle;
Utils::Animation* GameHelpers::gun_shot;

Texture2D GameHelpers::player_spritesheet;
Image GameHelpers::player_spritesheet_image;
std::vector<Utils::AnimationController> GameHelpers::player_controllers;
std::vector<Utils::AnimationController> GameHelpers::gun_controllers;
size_t GameHelpers::player_idle_idx;
size_t GameHelpers::player_moving_idx;
size_t GameHelpers::gun_idle_idx;
size_t GameHelpers::gun_shot_idx;
Texture2D GameHelpers::map;
Image GameHelpers::map_image;


