#ifndef H_PAGES_SPLASH_UI
#define H_PAGES_SPLASH_UI

#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct SplashUI: public UI::Elements::PageView{
    public:
        SplashUI();
        
        UI::Elements::Input* address_input;
        UI::Elements::Span* connect_button;
        std::string error_message;
    };
} // namespace Pages


#endif