#include "button.hpp"

DragonLib::UI::Elements::Span *Pages::Components::create_span_button(const std::string text, DragonLib::UI::DrawParameters::Measurement position, DragonLib::UI::DrawParameters::Measurement origin, float font_size)
{
    namespace UI = DragonLib::UI;
    using Mode = UI::DrawParameters::SizeMode;
    using namespace ColorScheme;
    
    return new UI::Elements::Span(
        [position, origin, text, font_size](UI::Elements::Span* button){
            return UI::DrawParameters::TextBox{
                .content = text,
                .font_size = font_size,
                .font_color = (button->hstate.hovered && button->interactable) ? alt_color_h : plain_color_h,
                .position = position,
                .rect_origin = origin,
                .fill = button->interactable ? (button->hstate.hovered ? (button->mouse_down ? plain_border_h : plain_border) : plain_fill) : plain_border_l,
                .border = {
                    .width = 2.0f,
                    .color = (button->hstate.hovered && button->interactable) ? plain_border : plain_fill,
                    .offset = button->interactable ? (button->mouse_down ? 10.0f : 12.0f) : 2.0f,
                },
                .padding = {
                    .value = {0.5, 0.2},
                    .mode = {Mode::SELF_H, Mode::SELF_H}
                }
            };
        },
        true
    );
}

DragonLib::UI::Elements::Text *Pages::Components::create_text_button(const std::string text, DragonLib::UI::DrawParameters::Measurement position, DragonLib::UI::DrawParameters::Measurement size, DragonLib::UI::DrawParameters::Measurement origin, float font_size)
{
    namespace UI = DragonLib::UI;
    using Mode = UI::DrawParameters::SizeMode;
    using namespace ColorScheme;

    return new UI::Elements::Text(
        [text, position, size, origin, font_size](UI::Elements::Text* button){
            return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                {
                    .position = position,
                    .size = size,
                    .origin = origin,
                    .fill = button->interactable ? (button->hstate.hovered ? (button->mouse_down ? plain_border_h : plain_border) : plain_fill) : plain_border_l,
                    .border = {
                        .width = 2.0f,
                        .color = (button->hstate.hovered && button->interactable) ? plain_border : plain_fill,
                        .offset = button->interactable ? (button->mouse_down ? 10.0f : 12.0f) : 2.0f,
                    }
                },
                {
                    .content = text,
                    .font_size = font_size,
                    .font_color = (button->hstate.hovered && button->interactable) ? alt_color_h : plain_color_h,
                    .position = {
                        .value={0.5, 0.5},
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    },
                    .origin = {
                        .value = {0.5, 0.5},
                        .mode = {Mode::SELF_W, Mode::SELF_H}
                    }
                }
            };
        },
        true
    );
}