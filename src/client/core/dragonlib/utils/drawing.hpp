#ifndef H_DLIB_DRAWING
#define H_DLIB_DRAWING

#include <assert.h>
#include <raylib.h>
#include "core/global.hpp"
#include "layout_calculation.hpp"
#include "core/dragonlib/layout_parameters.hpp"

namespace DragonLib
{
    namespace Utils
    {
        namespace Drawing
        {
            /**
             * Renders text (if wrap_width = 0, doesn't wrap)
             * 
             * RETURNS: The size of the bounding box of the text
             */
            Rectangle draw_text(UI::DrawParameters::Text params);

            /**
             * Renders text without wrap
             * NOTE: This is a helper function, you probably want to use draw_text
             */
            Rectangle __draw_text_no_wrap(UI::DrawParameters::Text& params);

            /**
             * Renders text with wrapping
             * NOTE: This is a helper function, you probably want to use draw_text
             */
            Rectangle __draw_text_with_wrap(UI::DrawParameters::Text& params);

            /**
             * Renders a rectangle
             * 
             * RETURNS: The size of the bounding box of the box
             */
            Rectangle draw_box(UI::DrawParameters::Box params);

            /**
             * Renders a rectangle without a border radius
             * NOTE: This is a helper function, you probably want to use draw_box
             * 
             * RETURNS: The size of the bounding box of the box
             */
            Rectangle __draw_box_no_radius(UI::DrawParameters::Box& params);

            /**
             * Renders a rectangle with border radius
             * NOTE: This is less efficient than no border radius
             * NOTE: This is a helper function, you probably want to use draw_box
             * 
             * RETURNS: The size of the bounding box of the box
             */
            Rectangle __draw_box_with_radius(UI::DrawParameters::Box& params);

            /**
             * Renders a box who size depends on the text inside it
             * NOTE: If you require a box whose size doesn't depend on text inside, simply draw a rect followed by the text :)
             */
            Rectangle draw_textbox(UI::DrawParameters::TextBox params);

            /**
             * Draws text at a given position.
             */
            void place_text(const char* content, Vector2 position, Color font_color, float font_size, Font* font = nullptr, bool sdf = true, float line_space = 1, float spacing = 1);
        } // namespace Drawing
        
    } // namespace Utils
    
} // namespace DragonLib

#endif