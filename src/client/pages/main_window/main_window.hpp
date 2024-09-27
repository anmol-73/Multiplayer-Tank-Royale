#ifndef HEADER_MAIN_WINDOW
#define HEADER_MAIN_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"
#include "core/global.hpp"
#include "core/scamlib/textbox.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{
        void _update();
        void _loading_update();
        private:
        struct UIState{
            bool start_button_is_hovered = false;
        } ui_state;
    };
} // namespace Pages
#endif