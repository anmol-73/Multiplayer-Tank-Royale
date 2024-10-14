#include "element.hpp"

void DragonLib::UI::Elements::Element::reset()
{
    clear_state();
}


void DragonLib::UI::Elements::Element::clear_state()
{
    hstate.clear();
}

inline bool DragonLib::UI::Elements::Element::HoverState::exclusively_hovered()
{
    return hovered && (!child_hovered);
}

void DragonLib::UI::Elements::Element::HoverState::clear()
{
    child_hovered = false;
    hovered = false;
    requested_pointer = false;
}

DragonLib::UI::Elements::Element::~Element() = default;
void DragonLib::UI::Elements::Element::draw(){}
void DragonLib::UI::Elements::Element::poll_events(){}
