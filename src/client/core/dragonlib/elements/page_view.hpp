#ifndef H_DLIB_ELEMENT_PAGE_VIEW
#define H_DLIB_ELEMENT_PAGE_VIEW

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