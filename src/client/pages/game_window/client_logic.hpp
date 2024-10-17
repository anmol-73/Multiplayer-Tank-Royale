#ifndef HEADER_CLIENT_LOGIC
#define HEADER_CLIENT_LOGIC
#include "common/maps/maps.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>

class LogicUtils{
    public:
        /*
        * The struct that enet will be using for communication  
        */
        struct PlayerPacket{
            int ID;
            Vector2 position_absolute;
            std::string name;
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

        MapData current_map;

        
        struct WallData
        {
            float width;
            int num_x;
            int num_y;
            std::vector<int>* walls;
        } wall_data;
        
        struct LivePlayerData{
            int ID;
            Vector2 position_absolute;
            std::string name;
            float player_angle;
            int health;
            int ammo;
            int max_ammo;
            bool is_alive;
        }player_data;

        /*
        * Define the Gun stats 
        */
        struct GunStats{
            Rectangle size = {
                .x = 0 ,
                .y = 30,
                .width = 20,
                .height = 80
            };
            bool has_shot;
            int gun_dmg;
            float gun_angle;
            float expected_gun_angle;
            double gun_rot_speed = 1;
            int gun_type;
        }gun_data;

        Vector2 mouse;  //stores mouse location
        Vector2 offset; //idk what this is for 

        struct HullStats{
            Rectangle size = {
                .width = 70,
                .height = 100,
            };
            int player_speed;
            double player_rot_speed = PI/2;
            int hull_type;
        } hull_data;

        struct CrosshairData
        {
            double mouse_distance;
            Vector2 tracker_position;
            double tracker_distance;
            const float tracker_radius = 10;
            double tracker_radial_speed = 500;
            Color circle_color = WHITE; 
        } crosshair_data;

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