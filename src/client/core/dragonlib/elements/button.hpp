#ifndef HEADER_CORE_DRAGONLIB_ELEMENTS_BUTTON
#define HEADER_CORE_DRAGONLIB_ELEMENTS_BUTTON

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
            struct Button: public Element{
            public:
                Button(std::function<DrawParameters::TextBox(Button*)> draw_param_provider_);
                void draw();
                void poll_events();
                void clear_state();
                
                bool clicked = false;
            private:
                Rectangle bounds = {};
                std::function<DrawParameters::TextBox(Button*)> draw_param_provider = {};
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif