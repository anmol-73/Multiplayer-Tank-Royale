#ifndef HEADER_GLOBAL
#define HEADER_GLOBAL

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
            extern Shader sdf_font_shader;
        } // namespace Shaders

        namespace Fonts
        {
            /**
             * The main font that the application shall use
             */
            extern Font main_sdf;
            
            /**
             * The main font that the application shall use
             * NOTE: It doesn't uses sdf
             */
            extern Font main;
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

#endif