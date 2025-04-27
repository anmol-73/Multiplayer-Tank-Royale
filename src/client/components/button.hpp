#ifndef H_COMPONENTS_BUTTON
#define H_COMPONENTS_BUTTON

#include "core/dragonlib.hpp"
#include "color_scheme.hpp"

namespace Pages
{    
    namespace Components
    {
        DragonLib::UI::Elements::Span* create_span_button(
            const std::string text,
            DragonLib::UI::DrawParameters::Measurement position,
            DragonLib::UI::DrawParameters::Measurement origin = {
                .value = {0.5, 0.5},
                .mode = {DragonLib::UI::DrawParameters::SizeMode::SELF_W, DragonLib::UI::DrawParameters::SizeMode::SELF_H}
            },
            float font_size = Global::rem
        );
        DragonLib::UI::Elements::Text* create_text_button(
            const std::string text,
            DragonLib::UI::DrawParameters::Measurement position,
            DragonLib::UI::DrawParameters::Measurement size,
            DragonLib::UI::DrawParameters::Measurement origin = {
                .value = {0.5, 0.5},
                .mode = {DragonLib::UI::DrawParameters::SizeMode::SELF_W, DragonLib::UI::DrawParameters::SizeMode::SELF_H}
            },
            float font_size = Global::rem
        );
    } // namespace Components
} // namespace Pages


#endif