#ifndef HEADER_PAGE_VIEW
#define HEADER_PAGE_VIEW

#include "view.hpp"
#include <raylib.h>

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            struct PageView: public View{
            public:
                void draw();
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib

#endif