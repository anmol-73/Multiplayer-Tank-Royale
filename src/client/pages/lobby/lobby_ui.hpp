#ifndef H_PAGES_LOBBY_UI
#define H_PAGES_LOBBY_UI

#include <functional>

#include "core/dragonlib.hpp"
#include "components.hpp"
#include "communication/protocol.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct LobbyUI: public UI::Elements::PageView{
    public:
        LobbyUI();
        
        void set_room_provider(std::function<const std::vector<Communication::Lobby::RoomDetail>&()> provider);
        std::string new_room_request();

    private:
        std::function<const std::vector<Communication::Lobby::RoomDetail>&()> room_provider;
        DragonLib::UI::Elements::Span *create_room_button;
        DragonLib::UI::Elements::Input *create_room_name_input;
    };
} // namespace Pages


#endif