#pragma once

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <mutex>
#include <list>

#include "core/scene_management/scene.hpp"
#include "core/scene_management/scene_management.hpp"
#include "core/scamlib/textbox.hpp"
#include "core/scamlib/text.hpp"
#include "core/maths.hpp"


namespace Scenes{
    class GameWindowScene: public Scene{
    public:
        enum GameState{
            PRE_GAME,
            GAME,
            POST_GAME
        };
        void update();
        void loading_update();
        void load_without_context();
        void unload_without_context();

    
    private:
        const Color player_color = GOLD;
        const unsigned int player_radius = 20;
        
        const Color gun_color = BLACK;
        const float gun_length = 25;

        const Color enemy_color = PINK;
        const unsigned int enemy_radius = 10;

        const Color drop_color = ORANGE;
        const unsigned int drop_radius = 5;

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

        GameState state;

        /**
         * The speed at which enemies move towards the player
         */
        float enemy_speed;
        float player_speed;

        unsigned long long int kills;

        /**
         * Basically a delta added to all coordinates :)
         */
        Vector2 camera;

        /**
         * The position of the player
         */
        Vector2 player;

        /**
         * The number of bullets left
         */
        unsigned int ammo;

        /**
         * Position of all enemies
         */
        std::vector<Vector2> enemies;

        /**
         * Position of all ammo drops
         */
        std::list<Vector2> drops;

        /**
         * Gets a random position to spawn an enemy at
         */
        Vector2 spawn_enemy();

        struct Data{
            Vector2 offset;
            double gun_angle;
            double sin_gun;
            double cos_gun;
            bool has_shot = false;
        } data;

        /**
         * Generates a random number for the amount of ammo to be dropped
         */
        unsigned int get_drop_amount();

        std::mutex load_unload_mutex;

        float pregame_timer;
        struct UIState{
            bool play_again_button_is_hovered = false;
            bool go_home_button_is_hovered = false;
        } ui_state;
    };

}

#include "game_window_scene.cpp"
