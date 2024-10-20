#ifndef HEADER_PAGES_LOBBY_WINDOW_UI
#define HEADER_PAGES_LOBBY_WINDOW_UI

#include "core/dragonlib.hpp"
#include <optional>

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct LobbyWindowUI: public UI::Elements::PageView{
    public:
        LobbyWindowUI();

        void make_ready(std::vector<std::pair<size_t, std::string>>& room_members);
    private:
        std::vector<std::pair<size_t, std::string>> room_members;
        std::vector<size_t> name_ids;
    };
} // namespace Pages


#endif