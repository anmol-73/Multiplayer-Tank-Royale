#include "button.hpp"


DragonLib::UI::Elements::Span *Pages::Components::create_span_button(const std::string text, DragonLib::UI::DrawParameters::Measurement position, DragonLib::UI::DrawParameters::Measurement origin, float font_size)
{
    namespace UI = DragonLib::UI;
    using Mode = UI::DrawParameters::SizeMode;
    return new UI::Elements::Span(
        [position, origin, text, font_size](UI::Elements::Span* button){
            return UI::DrawParameters::TextBox{
                .content = text,
                .font_size = font_size,
                .font_color = (button->hstate.hovered && button->interactable) ? Color{0xbf, 0xba, 0xa3, 0xe0} : Color{0x45, 0x41, 0x39, 0xe0},
                .position = position,
                .rect_origin = origin,
                .fill = button->interactable ? (button->hstate.hovered ? Color{0x49, 0x47, 0x3f, (unsigned char)(button->mouse_down ? 0xff : 0xc0)} : Color{0xb2, 0xad, 0x99, 0xc0}) : Color{0x49, 0x47, 0x3f, 0x70},
                .border = {
                    .width = 2.0f,
                    .color = (button->hstate.hovered && button->interactable) ? Color{0x49, 0x47, 0x3f, 0xc0} : Color{0xb2, 0xad, 0x99, 0xc0},
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
    return new UI::Elements::Text(
        [text, position, size, origin, font_size](UI::Elements::Text* button){
            return std::pair<UI::DrawParameters::Box, UI::DrawParameters::Text>{
                {
                    .position = position,
                    .size = size,
                    .origin = origin,
                    .fill = button->interactable ? (button->hstate.hovered ? Color{0x49, 0x47, 0x3f, (unsigned char)(button->mouse_down ? 0xff : 0xc0)} : Color{0xb2, 0xad, 0x99, 0xc0}) : Color{0x49, 0x47, 0x3f, 0x70},
                    .border = {
                        .width = 2.0f,
                        .color = (button->hstate.hovered && button->interactable) ? Color{0x49, 0x47, 0x3f, 0xc0} : Color{0xb2, 0xad, 0x99, 0xc0},
                        .offset = button->interactable ? (button->mouse_down ? 10.0f : 12.0f) : 2.0f,
                    }
                },
                {
                    .content = text,
                    .font_size = font_size,
                    .font_color = (button->hstate.hovered && button->interactable) ? Color{0xbf, 0xba, 0xa3, 0xe0} : Color{0x45, 0x41, 0x39, 0xe0},
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
