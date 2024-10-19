#include "button.hpp"

DragonLib::UI::Elements::Button::Button(std::function<DrawParameters::TextBox(Button *)> draw_param_provider_)
{
    draw_param_provider = draw_param_provider_;
}

void DragonLib::UI::Elements::Button::draw()
{
    bounds = Utils::Drawing::draw_textbox(draw_param_provider(this));
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
