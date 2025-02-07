#ifndef H_DLIB_LAYOUT_CALC
#define H_DLIB_LAYOUT_CALC

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