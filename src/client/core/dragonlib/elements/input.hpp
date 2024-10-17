#ifndef HEADER_CORE_DRAGONLIB_ELEMENTS_INPUT
#define HEADER_CORE_DRAGONLIB_ELEMENTS_INPUT

#include <string>
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
            struct InputParameters{
                DrawParameters::Measurement position = {
                    .value = {0.5, 0.5},
                    .mode = {DrawParameters::SizeMode::SCREEN_W, DrawParameters::SizeMode::SCREEN_H}
                };
                DrawParameters::Measurement origin = { // Specifies the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {DrawParameters::SizeMode::SELF_W, DrawParameters::SizeMode::SELF_H}
                };
                DrawParameters::Measurement size;
                DrawParameters::Measurement padding = { // Padding to add on the left and the right. NOTE: SELF is wrt the content rect!
                    .value = {0},
                    .mode = {DrawParameters::SizeMode::SELF_W, DrawParameters::SizeMode::SELF_H}
                };
                DrawParameters::Measurement text_origin = { // Specifies the origin of the text wrt to the rect origin. NOTE: SELF is wrt the content rect!
                    .value = {0},
                    .mode = {DrawParameters::SizeMode::SELF_W, DrawParameters::SizeMode::SELF_H}
                };
                
                Color background_color = WHITE;
                Color font_color = BLACK;

                Font* font; // The font (uses `Global::Rendering::Fonts::main` if set to null)
                bool sdf = true;
               
                DrawParameters::BorderParameters default_border_params;
                DrawParameters::BorderParameters focused_border_params;
                DrawParameters::BorderParameters hover_border_params;

                struct PlaceholderParameters{
                    std::string content;
                    Color placeholder_font_color = GRAY;
                } placeholder;
            };
            struct Input: public Element{
            public:
                Input(InputParameters parameters);

                void draw();
                void poll_events();
                void clear_state();
                
                std::string value;
            private:
                Rectangle bounds = {};
                InputParameters params;

                bool keyboard_focused = false;
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif