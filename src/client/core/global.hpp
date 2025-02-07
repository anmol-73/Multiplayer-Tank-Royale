#ifndef H_GLOBAL
#define H_GLOBAL

#include <raylib.h>
#include "dragonlib/utils/text.hpp"
#include "misc/task.hpp"

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
     * Just some unit proportional to screen height. Use it for like text size and all.
     */
    extern float rem;    

    /**
     * Initializes global parameters
     */
    void init();

    /**
     * Frees resources associated with global shit
     */
    void cleanup();
} // namespace Global

#endif