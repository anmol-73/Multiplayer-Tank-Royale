#ifndef HEADER_PAGES_LOBBY_WINDOW_UI
#define HEADER_PAGES_LOBBY_WINDOW_UI

#include "core/dragonlib.hpp"
#include "pages/componenents.hpp"
#include <optional>

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct LobbyWindowUI: public UI::Elements::PageView{
    public:
        LobbyWindowUI();

        void make_ready(std::vector<std::string> map_names_, bool is_leader);
        void poll_events();

        void update_room_members(std::vector<std::string>& room_members_);
        
        int current_map_idx = 0;
        UI::Elements::Span* start_game_button;
        UI::Elements::Text* left_map_select;
        UI::Elements::Text* right_map_select;
    private:
        
        std::vector<std::pair<size_t, std::string>> room_members;
        
        std::vector<std::string> map_names;
        

        
    };
} // namespace Pages


#endif