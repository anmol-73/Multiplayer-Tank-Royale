#include "span.hpp"

DragonLib::UI::Elements::Span::Span(std::function<DrawParameters::TextBox(Span *)> draw_param_provider_, bool interactable_)
{
    draw_param_provider = draw_param_provider_;
    interactable = interactable_;
}


void DragonLib::UI::Elements::Span::draw()
{
    bounds = Utils::Drawing::draw_textbox(draw_param_provider(this));
}

void DragonLib::UI::Elements::Span::poll_events()
{
    clear_state();
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hstate.hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && interactable;
    mouse_down = hstate.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && interactable;
    hstate.requested_pointer = hstate.hovered && interactable;
}

void DragonLib::UI::Elements::Span::clear_state()
{
    Element::clear_state();
    clicked = false;
    mouse_down = false;
}
