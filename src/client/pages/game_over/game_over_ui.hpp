#ifndef H_PAGES_GOVER_UI
#define H_PAGES_GOVER_UI

#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct GameOverUI: public UI::Elements::PageView{
    public:
        GameOverUI();
        
        std::vector<std::pair<std::string, int>> scores;
        UI::Elements::Span* go_to_lobby;
    };
} // namespace Pages


#endif