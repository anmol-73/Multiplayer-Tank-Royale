#include "page_view.hpp"

void DragonLib::UI::Elements::PageView::draw()
{
    if (!visible) return;
    View::draw();
    if (View::hstate.hovered){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}