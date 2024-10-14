#include "main_window_ui.hpp"

Pages::MainWindowUI::MainWindowUI()
{
    register_element(
        new UI::Elements::Button(
            [](UI::Elements::Button* buttons){
                return UI::DrawParameters::TextBox{
                    .content = "hello",
                    .font_size = 32,
                    .position = {0.5, 0.5},
                    .rect_origin = {0.5, 0.5},
                    .fill = buttons->hstate.hovered ? RED : YELLOW
                };
            }
        )
    );
}