#ifndef HEADER_PAGES_GAME_WINDOW
#define HEADER_PAGES_GAME_WINDOW

#include "maps/maps.hpp"
#include <raylib.h>
#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "utils/task.hpp"
#include "client_logic.hpp"
#include "core/utils/animation_controller.hpp"
#include "core/dragonlib.hpp"

namespace Pages
{
    class GameWindowScene: public SceneManagement::Scene{
    private:
        void _update();
        void _loading_update();

        void _load_with_context();
        void _cleanup_with_context();
        void _load();
        void _cleanup();

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
    
        Texture2D player_spritesheet;
        Image player_spritesheet_image;

        Utils::AnimationController* player_controller;
        size_t player_idle_idx;
        size_t player_moving_idx;

        Utils::AnimationController* gun_controller;
        size_t gun_idle_idx;
        size_t gun_shot_idx;
    };

} // namespace Pages


#endif