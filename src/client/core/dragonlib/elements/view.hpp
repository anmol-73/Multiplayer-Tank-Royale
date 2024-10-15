#ifndef HEADER_CORE_DRAGONLIB_ELEMENTS_VIEW
#define HEADER_CORE_DRAGONLIB_ELEMENTS_VIEW
#include <vector>
#include <memory>
#include "element.hpp"

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            struct View: public Element{
            public:
                void draw();

                void poll_events();

                size_t register_element(Element* element);

                void reset();

                void hide();

                void show();

                inline void disable_events();

                inline void enable_events();

            protected:
                std::vector<std::unique_ptr<Element>> elements;
                bool events_enabled = true;
                bool visible = true;
            };
        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib


#endif