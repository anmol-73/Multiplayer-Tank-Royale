#ifndef H_DLIB_ELEMENT_INPUT
#define H_DLIB_ELEMENT_INPUT

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
                std::string label = "";
                Color lable_color = BLACK;

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

                size_t max_input_size = 32;
                bool allow_newline = false;
            };
            struct Input: public Element{
            public:
                Input(InputParameters parameters);

                void draw();
                void poll_events();
                void clear_state();
                
                std::string value;
                bool keyboard_focused = false;
            private:
                Rectangle bounds = {};
                InputParameters params;

            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif