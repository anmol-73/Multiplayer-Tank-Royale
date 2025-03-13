#ifndef H_PAGES_LOBBY_UI
#define H_PAGES_LOBBY_UI

#include <functional>
#include <optional>

#include "misc/string_algorithms.hpp"
#include "core/dragonlib.hpp"
#include "components.hpp"
#include "communication/protocol.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct LobbyUI: public UI::Elements::PageView{
    public:
        LobbyUI();
        
        std::optional<Communication::Lobby::RoomDetail> join_room_request();
        std::string new_room_request();

        std::vector<Communication::Lobby::RoomDetail> visible_rooms;
        std::string error_message;
        void poll_events();

        std::string address_string;

    private:
        DragonLib::UI::Elements::Span *create_room_button;
        DragonLib::UI::Elements::Input *create_room_name_input;
        std::vector<DragonLib::UI::Elements::Text*> room_selects;
    };
} // namespace Pages


#endif