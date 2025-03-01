#ifndef H_PAGES_ROOM_UI
#define H_PAGES_ROOM_UI

#include <functional>
#include <optional>

#include "misc/string_algorithms.hpp"
#include "communication/protocol.hpp"
#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct RoomUI: public UI::Elements::PageView{
    public:
        RoomUI();

        bool should_start_game();
        bool should_leave();
        std::string set_name_request();

        int get_map_idx_delta();
        
        std::string room_name;
        std::vector<Communication::Room::PlayerDetail> visible_players;
        int map_idx;

    private:
        UI::Elements::Span* start_game_button;
        UI::Elements::Span* exit_button;
        UI::Elements::Text* left_map_select;
        UI::Elements::Text* right_map_select;

        DragonLib::UI::Elements::Span *set_name_button;
        DragonLib::UI::Elements::Input *name_input;
        
    };
} // namespace Pages


#endif