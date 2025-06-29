#ifndef H_DLIB_ELEMENT_TEXT
#define H_DLIB_ELEMENT_TEXT

#include <raylib.h>
#include <functional>
#include "element.hpp"
#include "core/dragonlib/utils.hpp"
#include "core/dragonlib/layout_parameters.hpp"

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            struct Text: public Element{
            public:
                Text(std::function<std::pair<DrawParameters::Box, DrawParameters::Text>(Text*)> draw_param_provider_, bool interactable_ = false);
                void draw();
                void poll_events();
                void clear_state();
                
                bool clicked = false;
                bool mouse_down = false;
                bool interactable;
                bool hidden = false;
            private:
                Rectangle bounds = {};
                std::function<std::pair<DrawParameters::Box, DrawParameters::Text>(Text*)> draw_param_provider = {};
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif