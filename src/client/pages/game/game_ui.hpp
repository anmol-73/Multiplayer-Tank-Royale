#ifndef H_PAGES_GAME_WINDOW_UI
#define H_PAGES_GAME_WINDOW_UI

#include <functional>
#include <optional>

#include "misc/string_algorithms.hpp"
#include "communication/protocol.hpp"
#include "core/dragonlib.hpp"
#include "game_constants.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct GameUI: public UI::Elements::PageView{
    public:
        GameUI();

        void allow_respawn(bool ok);
        bool should_respawn();

        void load_async();
        void load_sync();
        void cleanup_async();
        void cleanup_sync();

        void poll_events();


        int tank_idx = 0;
        int turret_idx = 0;
    private:
        DragonLib::UI::Elements::Span *respawn_button;

        std::vector<DragonLib::DImage> tank_images;
        std::vector<DragonLib::DImage> turret_images;

        UI::Elements::Text* left_tank_select;
        UI::Elements::Text* right_tank_select;
        UI::Elements::ImageView* tank_preview;

        UI::Elements::Text* left_turret_select;
        UI::Elements::Text* right_turret_select;
        UI::Elements::ImageView* turret_preview;

                
    };
} // namespace Pages


#endif