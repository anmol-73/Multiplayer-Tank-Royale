#ifndef H_PAGES_GAME_LOGIC_HELPERS
#define H_PAGES_GAME_LOGIC_HELPERS
#include "maps/maps.hpp"
#include "core/utils/animation_controller.hpp"
#include "collisions.hpp"
#include "core/global.hpp"
#include "game.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>

namespace GameHelpers{
    /*
    * The struct that enet will be using for communication  
    */

    struct PlayerInfo
    {
        std::string name;
        int score;
    };

    std::vector<PlayerInfo> extern leaderboard;

    struct CrosshairData
    {
        Vector2 mouse_position; // Relative
        double mouse_distance; // Relative
        Vector2 tracker_position; // Relative and wrt top left
        double tracker_distance; // Relative
        double tracker_radius;
        double tracker_radial_speed;
        Color circle_color = WHITE; 

        void init();
    } extern crosshair_data;

    /**
     * To update tracker circle to after delta_time
     */
    void set_tracker(float delta_time);

    Texture2D extern player_spritesheet;
    Image extern player_spritesheet_image;
    
    extern std::vector<Utils::AnimationController> player_controllers;
    size_t extern player_idle_idx;
    size_t extern player_moving_idx;

    extern std::vector<Utils::AnimationController> gun_controllers;
    size_t extern gun_idle_idx;
    size_t extern gun_shot_idx;

    extern int map_idx;

    extern Utils::Animation *player_idle;
    extern Utils::Animation *player_moving;
    extern Utils::Animation *gun_idle;
    extern Utils::Animation *gun_shot;

    Texture2D extern map;
    Image extern map_image;
};


#endif