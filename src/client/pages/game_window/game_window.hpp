#ifndef HEADER_PAGES_GAME_WINDOW
#define HEADER_PAGES_GAME_WINDOW

#include <mutex>
#include <condition_variable>

#include "maps/maps.hpp"
#include <raylib.h>
#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "core/task.hpp"
#include "client_logic.hpp"
#include "core/utils/animation_controller.hpp"
#include "core/utils/camera.hpp"
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
        void draw_leaderboard();
        

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

        void draw_name_health(int i);
        Rectangle health_bar;
        int max_health = 400;

        double pixels_per_unit_x;
        double pixels_per_unit_y;

        double time_since_last_send;
        Vector2 contact_point;
        bool bullet_colliding;
        Utils::Camera camera;
        // std::vector<LogicUtils::PlayerPacket> old_state;
        std::vector<LogicUtils::PlayerInfo> leaderboard;
        
    };

} // namespace Pages


#endif