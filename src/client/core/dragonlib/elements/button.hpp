#ifndef ELEMENTS_BUTTON
#define ELEMENTS_BUTTON

#include <raylib.h>
#include "element.hpp"
#include "core/dragonlib/utils.hpp"

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            struct Button: public Element{
            public:
                void draw();
                void poll_events();
                void clear_state();
                
                bool clicked = false;
            private:
                Rectangle bounds;
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib



#endif