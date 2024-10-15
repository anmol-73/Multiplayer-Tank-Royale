#ifndef HEADER_PAGES_MAIN_WINDOW_UI
#define HEADER_PAGES_MAIN_WINDOW_UI

#include "core/dragonlib.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct MainWindowUI: public UI::Elements::PageView{
    public:
        MainWindowUI();
    };
} // namespace Pages


#endif