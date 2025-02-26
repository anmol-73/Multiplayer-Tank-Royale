#ifndef H_COMPONENTS_SLINPUT
#define H_COMPONENTS_SLINPUT

#include "core/dragonlib.hpp"

namespace Pages
{
    namespace Components
    {
        /**
         * Creates a single line input :)
         */
        DragonLib::UI::Elements::Input* create_sl_input(
            const std::string& label, 
            const std::string& placeholder_text, 
            DragonLib::UI::DrawParameters::Measurement position, 
            DragonLib::UI::DrawParameters::Measurement size, 
            size_t max_input_size
        );
    } // namespace Components
    
} // namespace Pages


#endif