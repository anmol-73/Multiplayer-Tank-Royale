#ifndef HEADER_PAGES_MAIN_WINDOW
#define HEADER_PAGES_MAIN_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"
#include "main_window_ui.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{
        void _update();
        void _loading_update();


        MainWindowUI ui;
    };    
} // namespace Pages
#endif