#ifndef HEADER_TEXT
#define HEADER_TEXT

#include <string>
#include <raylib.h>
#include <assert.h>
#include <raymath.h>

namespace DragonLib
{
    namespace Utils
    {
        /**
         * Mesaures the size the given text would take if drawn on screen
         */
        Vector2 measure_text(Font *font, const std::string& text, float font_size, float spacing, float line_space);
        
        /**
         * Loads a font that supports sdf
         */
        Font load_sdf(const char* ttf_file, int font_size = 64, int glyph_count = 95);
    } // namespace Utils    
} // namespace DragonLib

#endif