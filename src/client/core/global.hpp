#ifndef HEADER_CORE_GLOBAL
#define HEADER_CORE_GLOBAL

#include <raylib.h>
#include "dragonlib/utils/text.hpp"
#include "communication/room_client.hpp"

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

    namespace ServiceProviders
    {
        extern Communication::RoomClient room_client;
    } // namespace ServiceProviders
    

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