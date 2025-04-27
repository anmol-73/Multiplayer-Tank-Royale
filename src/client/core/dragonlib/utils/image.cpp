#include "image.hpp"
#include <iostream>

DragonLib::DImage::DImage(const std::string &path_)
{
    path = path_;
}

DragonLib::DImage::DImage() = default;

void DragonLib::DImage::load_im()
{
    if (is_im_loaded) return;
    im = LoadImage(path.c_str());
    is_im_loaded = true;
}

void DragonLib::DImage::unload_im()
{
    // FIXME! For some reason UnloadImage gives a segfault!
    // Temporary fix... I have moved the Unload when the texture is loaded
    if (!is_im_loaded) return;
    is_im_loaded = false;
    UnloadImage(im);
}

void DragonLib::DImage::load_tex()
{
    if (is_tex_loaded) return;
    assert(is_im_loaded);

    tex = LoadTextureFromImage(im);
    is_im_loaded = false;
    UnloadImage(im);
    is_tex_loaded = true;
}

void DragonLib::DImage::unload_tex()
{
    if (!is_tex_loaded) return;
    is_tex_loaded = false;
    UnloadTexture(tex);
}
