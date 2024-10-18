#ifndef HEADER_PAGES_MAIN_WINDOW
#define HEADER_PAGES_MAIN_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"
#include "main_window_ui.hpp"
#include "core/global.hpp"
#include "utils/task.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{
    private:
        void _update();
        void _loading_update();

        void _cleanup();

        MainWindowUI ui;
        Utils::Task worker;
    };    
} // namespace Pages
#endif