#include "view.hpp"

void DragonLib::UI::Elements::View::draw()
{
    if (!visible) return;

    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->draw();
    }
}

void DragonLib::UI::Elements::View::poll_events()
{
    if ((!visible) || (!events_enabled)) return;
    
    clear_state();
    for (size_t i = elements.size(); i > 0; --i){
        elements[i - 1]->poll_events();
        hstate.child_hovered |= elements[i - 1]->hstate.hovered;
        hstate.requested_pointer |= elements[i - 1]->hstate.requested_pointer;
    }
    hstate.hovered = hstate.child_hovered;
}

size_t DragonLib::UI::Elements::View::register_element(Element *element)
{
    elements.emplace_back(element);
    return elements.size() - 1;
}

void DragonLib::UI::Elements::View::reset()
{
    clear_state();
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->reset();
    }
}

void DragonLib::UI::Elements::View::hide()
{
    if (!visible) return;
    reset();
    visible = false;
}

void DragonLib::UI::Elements::View::show()
{
    visible = true;
}

inline void DragonLib::UI::Elements::View::disable_events()
{
    events_enabled = false;
}

inline void DragonLib::UI::Elements::View::enable_events()
{
    events_enabled = true;
}

void DragonLib::UI::Elements::View::load_async()
{
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->load_async();
    }
}

void DragonLib::UI::Elements::View::cleanup_async()
{
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->cleanup_async();
    }
}

void DragonLib::UI::Elements::View::load_sync()
{
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->load_sync();
    }
}

void DragonLib::UI::Elements::View::cleanup_sync()
{
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->cleanup_sync();
    }
}
