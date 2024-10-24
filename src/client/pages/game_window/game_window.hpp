#ifndef HEADER_PAGES_GAME_WINDOW
#define HEADER_PAGES_GAME_WINDOW

#include "maps/maps.hpp"
#include <raylib.h>
#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "core/task.hpp"
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

        double pixels_per_unit_x;
        double pixels_per_unit_y;

        double time_since_last_send;

        std::vector<LogicUtils::PlayerPacket> old_state;
    };

} // namespace Pages


#endif