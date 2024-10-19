#ifndef HEADER_PAGES_MAIN_WINDOW_UI
#define HEADER_PAGES_MAIN_WINDOW_UI

#include "core/dragonlib.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct MainWindowUI: public UI::Elements::PageView{
    public:
        MainWindowUI();

        const std::string& address_input_value();
        bool address_submit_requested();
        void show_error(std::string error);
        void poll_events();
    private:
        size_t address_input_id;
        size_t address_submit_id;
        std::string error_text;
    };
} // namespace Pages


#endif