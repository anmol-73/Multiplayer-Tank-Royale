#ifndef ELEMENTS_BUTTON
#define ELEMENTS_BUTTON

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
                Button(std::function<DrawParameters::TextBox(Button*)> draw_param_provider_) {
                    draw_param_provider = draw_param_provider_;
                    reactive = true;
                };
                Button(DrawParameters::TextBox draw_params_){
                    draw_params = draw_params_;
                    reactive = false;
                }

                void draw();
                void poll_events();
                void clear_state();
                
                bool clicked = false;
            private:
                Rectangle bounds = {};

                bool reactive = false;
                DrawParameters::TextBox draw_params = {};
                std::function<DrawParameters::TextBox(Button*)> draw_param_provider = {};
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif