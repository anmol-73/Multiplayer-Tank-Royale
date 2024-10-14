#ifndef HEADER_TEXT
#define HEADER_TEXT

#include <string>
#include <raylib.h>
#include <assert.h>
#include <raymath.h>
#include "core/global.hpp"
#include "layout_calculation.hpp"
#include "core/dragonlib/layout_parameters.hpp"

namespace DragonLib
{
    namespace Utils
    {
        namespace Text
        {
            /**
             * Mesaures the size the given text would take if drawn on screen
             */
            Vector2 measure_text(Font *font, const std::string& text, float font_size, float spacing, float line_space);
            
            /**
             * Loads a font that supports sdf
             */
            Font load_sdf(const char* ttf_file, int font_size = 64, int glyph_count = 95);

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
        } // namespace Text        
    } // namespace Utils    
} // namespace DragonLib

#endif