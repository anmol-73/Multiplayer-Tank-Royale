#include "text.hpp"

DragonLib::UI::Elements::Text::Text(std::function<std::pair<DrawParameters::Box,DrawParameters::Text>(Text*)> draw_param_provider_)
{
    draw_param_provider = draw_param_provider_;
}

void DragonLib::UI::Elements::Text::draw()
{
    auto [box_params, text_params] = draw_param_provider(this);
    bounds = Utils::Drawing::draw_box(box_params);

    text_params.position.value = Vector2Add(
        Utils::Calculation::resolve_measurement(box_params.position, {bounds.width, bounds.height}),
        Utils::Calculation::resolve_measurement(text_params.position, {bounds.width, bounds.height})
    );
    text_params.position.mode = {DrawParameters::SizeMode::ABSOLUTE, DrawParameters::SizeMode::ABSOLUTE};
    Utils::Drawing::draw_text(text_params);
}

void DragonLib::UI::Elements::Text::poll_events()
{
    clear_state();
    hstate.hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hstate.hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    mouse_down = hstate.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    hstate.requested_pointer = hstate.hovered;
}

void DragonLib::UI::Elements::Text::clear_state()
{
    Element::clear_state();
    clicked = false;
    mouse_down = false;
}
