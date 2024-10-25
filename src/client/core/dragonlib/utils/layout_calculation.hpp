#ifndef HEADER_CORE_DRAGONLIB_UTILS_LAYOUT_CALCULATION
#define HEADER_CORE_DRAGONLIB_UTILS_LAYOUT_CALCULATION

#include <assert.h>
#include "core/dragonlib/layout_parameters.hpp"

namespace DragonLib
{
    namespace Utils
    {
        namespace Calculation
        {
            Vector2 resolve_measurement(UI::DrawParameters::Measurement measurement, Vector2 element_size = {0});
        } // namespace Calculation        
    } // namespace Utils    
} // namespace DragonLib

#endif