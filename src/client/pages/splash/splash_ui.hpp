#ifndef H_PAGES_SPLASH_UI
#define H_PAGES_SPLASH_UI

#include <optional>
#include "core/dragonlib.hpp"
#include "components.hpp"
#include "communication/structs.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct SplashUI: public UI::Elements::PageView{
    public:
        SplashUI(); // some way to talk bw scenes would be nice.... hmmm
        
        /**
         * Returns the address provided by the user (if it is a valid address)
         */
        std::optional<Communication::Address> lobby_address();

    private:
        UI::Elements::Input* address_input;
    };
} // namespace Pages


#endif