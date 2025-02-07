#ifndef H_PAGES_SPLASH_UI
#define H_PAGES_SPLASH_UI

#include "core/dragonlib.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct MainWindowUI: public UI::Elements::PageView{
    public:
        MainWindowUI();

        void show_info(std::string text, bool is_error);
        UI::Elements::Input* name_input;
        UI::Elements::Input* address_input;
        UI::Elements::Span* address_submit_button;
    private:
        std::pair<bool, std::string> information;
    };
} // namespace Pages


#endif