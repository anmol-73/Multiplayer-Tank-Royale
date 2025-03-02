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
    };
} // namespace Pages


#endif