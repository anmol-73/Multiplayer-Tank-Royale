#ifndef HEADER_TEXT
#define HEADER_TEXT

#include "core\global.hpp"
#include <raylib.h>
#include <string>

namespace Scamlib
{
    namespace Draw
    {
        struct TextDrawParameters{
            std::string content; // The text content
            Vector2 position; // The position to draw in 
            Vector2 translate; // Additional translation as a percentage of the element size (basically a delta to the position)
            float wrap_width = 0; // Set to 0 if no wrapping to be done
            Color color = GRAY; // Font color
            float size = 32; // Font size
            Font *font = nullptr; // The font (uses `Global::Rendering::Fonts::main` if set to null)
            float spacing = 1; // The space between letters
            float line_space = 1.0f; // The vertical padding between lines of text (As a percentage of height of the text)
        };

        /**
         * Renders text (if wrap_width = 0, doesn't wrap)
         * 
         * RETURNS: The size of the bounding box of the text
         */
        template <bool sdf=true>
        Rectangle text(TextDrawParameters params);

        template <bool sdf=true>
        Font* default_font();

        /**
         * Renders text without wrap
         * NOTE: This is a helper function
         * 
         * RETURNS: The size of the bounding box of the text
         */
        template <bool sdf>
        Rectangle __text_no_wrap(TextDrawParameters& params);

        /**
         * Renders text with wrapping
         * NOTE: This is a helper function
         * 
         * RETURNS: The size of the bounding box of the text
         */
        template <bool sdf>
        Rectangle __text_with_wrap(TextDrawParameters& params);

        /**
         * Measure the size of text :)
         */
        Vector2 measure_text(Font* font, const std::string& text, float font_size, float spacing, float line_space);
    } // namespace Draw

    namespace Load{
        /**
         * Loads a font that uses sdf
         */
        Font font_sdf(const char* ttf_file, int font_size = 64, int glyph_count = 95);
    }
    
} // namespace Scamlib

#endif