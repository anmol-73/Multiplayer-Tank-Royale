#ifndef HEADER_CLIENT_LOGIC
#define HEADER_CLIENT_LOGIC
#include "maps/maps.hpp"
#include "core/utils/animation_controller.hpp"
#include "collisions.hpp"
#include "core/global.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#define SHOTCOOLDOWN 1

namespace LogicUtils{
    /*
    * The struct that enet will be using for communication  
    */
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
    } extern player_packet;

    std::vector<PlayerPacket> extern old_state;
    std::vector<PlayerPacket> extern old_state_buffer;

    struct PlayerInfo {
    std::string name;
    int score;
    };

    std::vector<PlayerInfo> extern leaderboard;



    /*
    * THe live player data used which updates every frame
    */
    
    struct LivePlayerData{
        Vector2 position; // Tank top left in units (absoolute)
        double angle; // Angle with x axis
        int health ;
        // int ammo;
        // int max_ammo;
        bool is_alive;

        void init(Vector2 position);
    } extern player_data;
    LivePlayerData extern projected_data;

    /*
    * Define the Gun stats 
    */
    struct GunStats{
        Rectangle gun_rectangle = {
            .width = 48,
            .height = 12
        };
        bool has_shot;
        int gun_dmg = 100;
        double gun_angle;
        double expected_gun_angle;
        double gun_rot_speed;
        double bullet_range;
        // int gun_type;
        void init();
    } extern gun_data;
    extern float timesince_lastshot;

    struct HullStats{
        Rectangle player_rectangle = {
            .width = 64,
            .height = 48,
        };
        double player_speed;
        double player_rot_speed;
        bool player_colliding;
        // int hull_type;

        void init();
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

        void init();
    } extern crosshair_data;

    struct ViewportData
    {
        Vector2 offset; // Top left
        Vector2 projected_offset;
    } extern viewport_data;

    void init_state(int max_players);

    void update_state(PlayerPacket *received_state);

    void set_packet();

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

    extern std::vector<float> old_timestamps;
    extern std::vector<bool> did_shoots;
    extern std::vector<Vector2> contact_pointsss;
    bool handle_tank_collision();

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