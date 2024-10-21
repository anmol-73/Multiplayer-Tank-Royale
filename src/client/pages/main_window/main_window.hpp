#ifndef HEADER_PAGES_MAIN_WINDOW
#define HEADER_PAGES_MAIN_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"
#include "core/dragonlib.hpp"
#include "main_window_ui.hpp"
#include "core/global.hpp"
#include "core/task.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{    
    public:
        MainWindowScene();
    private:
        void _update();
        void _loading_update();

        void _load_with_context();
        void _cleanup_with_context();
        void _load();
        void _cleanup();

        MainWindowUI ui;
        Utils::Task connect_worker;

        DragonLib::DImage background;
    };    
} // namespace Pages
#endif