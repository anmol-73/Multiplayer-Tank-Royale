#ifndef H_PAGES_ROOM_UI
#define H_PAGES_ROOM_UI

#include <functional>
#include <optional>

#include "misc/string_algorithms.hpp"
#include "communication/protocol.hpp"
#include "core/dragonlib.hpp"
#include "components.hpp"

namespace Pages
{
    namespace UI = DragonLib::UI;
    struct RoomUI: public UI::Elements::PageView{
    public:
        RoomUI();

    private:
        
    };
} // namespace Pages


#endif