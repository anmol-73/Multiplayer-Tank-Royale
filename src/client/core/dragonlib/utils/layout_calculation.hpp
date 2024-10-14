#ifndef TEMPLATE_CALCULATION
#define TEMPLATE_CALCULATION

#include <assert.h>
#include "core/dragonlib/layout_parameters.hpp"

namespace DragonLib
{
    namespace Utils
    {
        namespace Calculation
        {
            Vector2 resolve_item_units(Vector2& item, UI::DrawParameters::CalculationMode mode, Vector2 element_size = {0});
        } // namespace Calculation        
    } // namespace Utils    
} // namespace DragonLib

#endif