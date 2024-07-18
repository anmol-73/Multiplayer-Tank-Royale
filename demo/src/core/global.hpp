#pragma once

#include <raylib.h>

namespace Global
{
    namespace Rendering
    {
        namespace Shaders
        {
            /**
             * Shader to render fonts that use sdf
             */
            Shader sdf_font_shader;
        } // namespace Shaders

        namespace Fonts
        {
            /**
             * The main font that the application shall use
             */
            Font main_sdf;
            
            /**
             * The main font that the application shall use
             * NOTE: It doesn't uses sdf
             */
            Font main;
        } // namespace Fonts
    } // namespace Rendering

    /**
     * Initializes are global parameters
     */
    void init();

    /**
     * Frees resources associated with global shit
     */
    void free();  
} // namespace Global
