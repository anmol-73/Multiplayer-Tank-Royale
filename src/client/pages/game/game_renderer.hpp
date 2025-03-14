#ifndef H_PAGES_GAME_RENDERER
#define H_PAGES_GAME_RENDERER

#include "game.hpp"
#include "core/dragonlib.hpp"
#include "core/utils/camera.hpp"
#include "game_client.hpp"

#include "core/utils/animations/animation_controller.hpp"

namespace Pages
{
    struct GameRenderer{
    public:
        GameRenderer();

        void draw(Game::GameState& gs, int player_id, const std::vector<Communication::Game::PlayerIdentification>& pd);
        void draw_leaderboard(const Game::GameState& gs, const std::vector<Communication::Game::PlayerIdentification>& pd);

        bool game_started = false;
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
        /**
         * Called every time player respawns (as player_tank_type might change)
         */
        void prepare(size_t map_idx, size_t player_tank_type);
        
        void load_async();
        void load_sync();
        void cleanup_async();
        void cleanup_sync();

        Utils::Camera camera;
    private:
        
        size_t map_index = 0;
        size_t tank_index = 0;

        std::vector<DragonLib::DImage> map_images;

        std::vector<DragonLib::DImage> tank_spritesheets;
        std::vector<DragonLib::DImage> gun_spritesheets;
        DragonLib::DImage explosion_spritesheet;
        DragonLib::DImage skull_spritesheet;
        DragonLib::DImage projectile_spritesheet;
        DragonLib::DImage spacebg_spritesheet;

        Utils::EffectAnimationController effect_ac;

        std::vector<Utils::AnimationController> tank_acs;
        std::vector<Utils::AnimationController> gun_acs;


        std::vector<double> prev_times_since_last_shot;
    };
} // namespace Pages


#endif