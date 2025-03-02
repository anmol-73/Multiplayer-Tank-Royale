#include "image_view.hpp"
#include <iostream>

void DragonLib::UI::Elements::ImageView::draw()
{
    const auto params = draw_param_provider(this);
    const auto bbox = Utils::Drawing::draw_box(params);
    DrawTexturePro(
        image.tex,
        {0, 0, (float)image.tex.width, (float)image.tex.height},
        bbox,
        {0},
        0,
        WHITE
    );

    View::draw();
}

void DragonLib::UI::Elements::ImageView::load_async()
{
    image.load_im();
    View::load_async();
}

void DragonLib::UI::Elements::ImageView::cleanup_async()
{
    image.unload_im();
    View::cleanup_async();
}

void DragonLib::UI::Elements::ImageView::load_sync()
{
    image.load_tex();
    View::load_sync();
}

void DragonLib::UI::Elements::ImageView::cleanup_sync()
{
    image.unload_tex();
    View::cleanup_sync();
}
