#ifndef HEADER_CLIENT_LOGIC
#define HEADER_CLIENT_LOGIC
#include "maps/maps.hpp"
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
    }player_packet;

    /*
    * THe live player data used which updates every frame
    */
    
    struct LivePlayerData{
        int ID;
        Vector2 position;
        std::string name;
        double angle;
        int health;
        int ammo;
        int max_ammo;
        bool is_alive;
    }player_data;

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
    }gun_data;

    struct HullStats{
        Rectangle player_rectangle = {
            .width = 128,
            .height = 96,
        };
        double player_speed;
        double player_rot_speed;
        bool player_colliding;
        // int hull_type;
    } hull_data;

    struct CrosshairData
    {
        Vector2 mouse_position;
        double mouse_distance;
        Vector2 tracker_position;
        double tracker_distance;
        double tracker_radius;
        double tracker_radial_speed;
        Color circle_color = WHITE; 
    } crosshair_data;

    struct ViewportData
    {
        Vector2 offset; // Top left
    } viewport_data;

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
};


#endif