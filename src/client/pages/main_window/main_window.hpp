#pragma once
#include <raylib.h>

#include "core/scene_management.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{
        void _update();
        void _loading_update();
    };    
} // namespace Pages
