#include "text.hpp"


DragonLib::UI::Elements::Text::Text(std::function<std::pair<DrawParameters::Box, DrawParameters::Text>(Text *)> draw_param_provider_, bool interactable_)
{
    draw_param_provider = draw_param_provider_;
    interactable = interactable_;
}

void DragonLib::UI::Elements::Text::draw()
{
    if (hidden) return;
    auto [box_params, text_params] = draw_param_provider(this);
    bounds = Utils::Drawing::draw_box(box_params);

    text_params.position.value = Vector2Add(
        {bounds.x, bounds.y},
        Utils::Calculation::resolve_measurement(text_params.position, {bounds.width, bounds.height})
    );
    text_params.position.mode = {DrawParameters::SizeMode::ABSOLUTE, DrawParameters::SizeMode::ABSOLUTE};
    Utils::Drawing::draw_text(text_params);
}

void DragonLib::UI::Elements::Text::poll_events()
{
    clear_state();
    if (hidden) return;
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hstate.hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && interactable;
    mouse_down = hstate.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && interactable;
    hstate.requested_pointer = hstate.hovered && interactable;
}

void DragonLib::UI::Elements::Text::clear_state()
{
    Element::clear_state();
    clicked = false;
    mouse_down = false;
}
