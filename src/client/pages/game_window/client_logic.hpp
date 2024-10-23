#ifndef HEADER_CLIENT_LOGIC
#define HEADER_CLIENT_LOGIC
#include "maps/maps.hpp"
#include "core/utils/animation_controller.hpp"
#include "collisions.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>

namespace LogicUtils{
    /*
    * The struct that enet will be using for communication  
    */
    struct PlayerPacket{
        int ID;
        Vector2 position_absolute;
        float player_angle;
        float gun_angle;
        bool has_shot;
        int gun_dmg;
        int health;
        bool is_alive;
        bool is_connected;
    } extern player_packet;

    std::vector<PlayerPacket> extern old_state;

    /*
    * THe live player data used which updates every frame
    */
    
    struct LivePlayerData{
        Vector2 position; // Tank top left in units (absoolute)
        double angle; // Angle with x axis
        int health;
        // int ammo;
        // int max_ammo;
        bool is_alive;
    } extern player_data;
    LivePlayerData extern projected_data;

    /*
    * Define the Gun stats 
    */
    struct GunStats{
        Rectangle gun_rectangle = {
            .width = 128,
            .height = 32
        };
        bool has_shot;
        int gun_dmg;
        double gun_angle;
        double expected_gun_angle;
        double gun_rot_speed;
        double bullet_range;
        // int gun_type;
    } extern gun_data;

    struct HullStats{
        Rectangle player_rectangle = {
            .width = 128,
            .height = 96,
        };
        double player_speed;
        double player_rot_speed;
        bool player_colliding;
        // int hull_type;
    } extern hull_data;

    struct CrosshairData
    {
        Vector2 mouse_position; // Relative
        double mouse_distance; // Relative
        Vector2 tracker_position; // Relative and wrt top left
        double tracker_distance; // Relative
        double tracker_radius;
        double tracker_radial_speed;
        Color circle_color = WHITE; 
    } extern crosshair_data;

    struct ViewportData
    {
        Vector2 offset; // Top left
        Vector2 projected_offset;
    } extern viewport_data;

    void init_state(int max_players);

    double normalize_angle(double angle);
    /**
     * To update player_position based on button presses after delta_time
     */ 
    void handle_movement(float delta_time);
    /**
     * To update angle to after delta_time
     */
    void set_gun_angle(float delta_time);

    /**
     * To update tracker circle to after delta_time
     */
    void set_tracker(float delta_time);

    bool handle_tank_collision();

    Texture2D extern player_spritesheet;
    Image extern player_spritesheet_image;

    Utils::AnimationController extern *player_controller;
    size_t extern player_idle_idx;
    size_t extern player_moving_idx;

    Utils::AnimationController extern *gun_controller;
    size_t extern gun_idle_idx;
    size_t extern gun_shot_idx;

    Texture2D extern map;
    Image extern map_image;
};


#endif