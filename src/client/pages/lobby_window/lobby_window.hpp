#ifndef HEADER_PAGES_LOBBY_WINDOW
#define HEADER_PAGES_LOBBY_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"
#include "lobby_window_ui.hpp"
#include "core/global.hpp"
#include "core/task.hpp"

namespace Pages
{
    class LobbyWindowScene: public SceneManagement::Scene{
    public:
        LobbyWindowScene();
    private:

        void _update();
        void _loading_update();

        void _load_with_context();
        void _cleanup_with_context();
        void _load();
        void _cleanup();

        LobbyWindowUI ui;

        DragonLib::DImage background;
        std::vector<DragonLib::DImage> map_images;
        
        std::vector<std::string> room_members;
    };

} // namespace Pages
#endif