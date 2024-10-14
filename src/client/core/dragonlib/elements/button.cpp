#include "button.hpp"

void DragonLib::UI::Elements::Button::draw()
{
    std::string content = "123";
    Utils::Text::draw_text({
        .content = content,
        .font = &Global::Rendering::Fonts::main_sdf,
        .size = 96,
        .color = WHITE,
    });
}

void DragonLib::UI::Elements::Button::poll_events()
{
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hstate.hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void DragonLib::UI::Elements::Button::clear_state()
{
    Element::clear_state();
    clicked = false;
}
