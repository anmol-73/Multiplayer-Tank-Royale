#ifndef HEADER_GAME_WINDOW
#define HEADER_GAME_WINDOW

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <raylib.h>
#include <raymath.h>

#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "core/scamlib/textbox.hpp"
#include "core/scamlib/text.hpp"
#include "core/maths.hpp"
#include "core/utils/animation_controller.hpp"
#include "core/physics/collisions.hpp"

namespace Pages
{
    class GameWindowScene: public SceneManagement::Scene{
        void _update();
        void _loading_update();
        void _load();
        void _cleanup();
        void _load_with_context();
        void _cleanup_with_context();

        private:
                   const float bullet_range = 2000;

            /**
             * Handles updating game logic each frame
             * 
             * RETURNS: true if the drawing should be skipped this frame (For example if scene is changed)
             */
            void logic_update();

            /**
             * Draws out the game
             */
            void draw_game();

            /**
             * Draws the hud
             */
            void draw_hud();

            /**
             * Draws the floor
             */
            void draw_bg();  

            /**
             * Basically a delta added to all coordinates :)
             */
            Vector2 camera;

            /**
             * The player position
             */
            Vector2 player_position;

            /**
             * Offset 
             */
            Vector2 offset;

            /**
             * The mouse position
             */
            Vector2 mouse;

            /**
             * The player size, speed, angle and angular velocity
             */
            struct PlayerData
            {
                Rectangle player = {
                    .width = 70,
                    .height = 100,
                };
                float player_speed;
                double player_angle;
                double sin_player;
                double cos_player;
                double player_rot_speed = PI/2;
                bool player_colliding = false;
            } player_data;

            Utils::AnimationController* player_controller;
            size_t player_idle_idx;
            size_t player_moving_idx;

            Utils::AnimationController* gun_controller;
            size_t gun_idle_idx;
            size_t gun_shot_idx;

            Texture2D player_spritesheet;
            Image player_spritesheet_image;
            bool main_load_ready;

            /**
             * The gun size, angle, sincos, angular speed, state
             */
            struct GunData
            {
                Rectangle gun = {
                    .width = 20,
                    .height = 80
                };
                double gun_angle;
                double predicted_gun_angle;
                double sin_gun;
                double cos_gun;
                double gun_rot_speed = 1; // rads per sec
                bool has_shot = false;
            } gun_data;

            /**
             * The gun size, angle, sincos, angular speed, state
             */
            struct CrosshairData
            {
                double mouse_distance;
                Vector2 tracker_position;
                double tracker_distance;
                const float tracker_radius = 10;
                double tracker_radial_speed = 500;
                Color circle_color = WHITE; 
            } crosshair_data;

            struct WallData
            {
                float width;
                int num_x;
                int num_y;
                std::vector<std::pair<int, int>> walls = {std::make_pair(1, 2), std::make_pair(5,5)};
            } wall_data;

            Texture2D floor_tileset;
            Image floor_tileset_image;

            /**
             * To set gun and tank position to player_position
             */
            void set_position();

            /**
             * To update player_position and play animations based on button presses after delta_time
             */
            void handle_movement(float delta_time);

            /**
             * To make angle between pi and -pi
             */
            double normalize_angle(double angle);

            /**
             * To update angle to after delta_time
             */
            void set_gun_angle(float delta_time);

            /**
             * To update tracker circle to after delta_time
             */
            void set_tracker(float delta_time);

            /**
             * The number of bullets left
             */
            unsigned int ammo;

    };
} // namespace Pages

#endif
