#include "button.hpp"

void DragonLib::UI::Elements::Button::draw()
{
    if (reactive){
        draw_params = draw_param_provider(this);
    }
    bounds = Utils::Drawing::draw_textbox(draw_params);
}

void DragonLib::UI::Elements::Button::poll_events()
{
    clear_state();
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hstate.hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    hstate.requested_pointer = hstate.hovered;
}

void DragonLib::UI::Elements::Button::clear_state()
{
    Element::clear_state();
    clicked = false;
}
