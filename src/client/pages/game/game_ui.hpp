#ifndef H_PAGES_GAME_WINDOW_UI
#define H_PAGES_GAME_WINDOW_UI

#include <functional>
#include <optional>

#include "misc/string_algorithms.hpp"
#include "communication/protocol.hpp"
#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct GameUI: public UI::Elements::PageView{
    public:
        GameUI();

    private:
                
    };
} // namespace Pages


#endif