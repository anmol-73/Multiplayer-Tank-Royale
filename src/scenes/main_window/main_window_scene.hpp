#pragma once

#include <raylib.h>

#include "core/scene_management/scene.hpp"
#include "core/scene_management/scene_management.hpp"
#include "core/global.hpp"
#include "core/scamlib/textbox.hpp"

namespace Scenes{
    class MainWindowScene: public Scene{
    public:
        void update();
    private:
        struct UIState{
            bool start_button_is_hovered = false;
            bool other_button_is_hovered = false;
        } ui_state;
    };
}

#include "main_window_scene.cpp"
