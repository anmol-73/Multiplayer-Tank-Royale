#ifndef H_PAGES_GAME_RENDERER
#define H_PAGES_GAME_RENDERER

#include "game.hpp"
#include "core/dragonlib.hpp"
#include "core/utils/camera.hpp"

namespace Pages
{
    struct GameRenderer{
    public:
        GameRenderer();

        void draw(const Game::GameState& gs, int player_id);

        /**
         * Called every time player respawns (as player_tank_type might change)
         */
        void prepare(size_t map_idx, size_t player_tank_type);

        void load_async();
        void load_sync();
        void cleanup_async();
        void cleanup_sync();

    private:
        Utils::Camera camera;
        
        size_t map_index = 0;
        size_t tank_index = 0;

        std::vector<DragonLib::DImage> map_images;

        struct CrosshairData
        {
            Vector2 mouse_position; // Relative
            double mouse_distance; // Relative
            Vector2 tracker_position; // Relative and wrt top left
            double tracker_distance; // Relative
            double tracker_radius;
            double tracker_radial_speed;
            Color circle_color = RED; 
    
            void init();
        } crosshair_data;

        std::vector<double> prev_times_since_last_shot;
    };
} // namespace Pages


#endif