#pragma once

#include <raylib.h>
#include "text.hpp"
#include "box.hpp"

namespace Scamlib
{
    namespace Draw
    {
        /**
         * Highly ungeneric textbox
         * 
         * TODO: Make this muuuuch more generic
         * 
         * RETURNS: The size of the bounding box of the text
         */
        template <bool sdf=true>
        Rectangle textbox(TextDrawParameters text_params, BoxDrawParameters box_params, Vector2 padding = {});
    } // namespace Draw
    
} // namespace Scamlib

#include "textbox.cpp"
