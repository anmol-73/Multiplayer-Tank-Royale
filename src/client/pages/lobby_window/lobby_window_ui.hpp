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

        void make_ready(std::vector<std::pair<size_t, std::string>>& room_members_, std::vector<std::string> &map_names_);
        void poll_events();
        
        int current_map_idx = 0;
    private:
        std::vector<std::pair<size_t, std::string>> room_members;
        std::vector<size_t> name_ids;
        std::vector<std::string> map_names;
        size_t start_game_button_id;
        size_t left_map_idx;
        size_t right_map_idx;

        
    };
} // namespace Pages


#endif