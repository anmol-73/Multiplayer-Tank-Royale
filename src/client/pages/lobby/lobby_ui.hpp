#ifndef H_PAGES_LOBBY_UI
#define H_PAGES_LOBBY_UI

#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct LobbyUI: public UI::Elements::PageView{
    public:
        LobbyUI();
    };
} // namespace Pages


#endif