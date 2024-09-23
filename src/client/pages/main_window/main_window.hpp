#ifndef HEADER_MAIN_WINDOW
#define HEADER_MAIN_WINDOW
#include <raylib.h>

#include "core/scene_management.hpp"

namespace Pages
{
    class MainWindowScene: public SceneManagement::Scene{
        void _update();
        void _loading_update();
    };    
} // namespace Pages
#endif