#include "page_view.hpp"

void DragonLib::UI::Elements::PageView::draw()
{
    if (!visible) return;
    
    for (size_t i = 0; i < elements.size(); ++i){
        elements[i]->draw();
    }

    if (View::hstate.requested_pointer){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}