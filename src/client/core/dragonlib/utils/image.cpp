#include "image.hpp"

DragonLib::DImage::DImage(const std::string &path_)
{
    path = path_;
}

DragonLib::DImage::DImage() = default;

void DragonLib::DImage::load_im()
{
    if (is_im_loaded) return;
    is_im_loaded = true;
    im = LoadImage(path.c_str());
}

void DragonLib::DImage::unload_im()
{
    if (!is_im_loaded) return;
    is_im_loaded = false;
    UnloadImage(im);
}

void DragonLib::DImage::load_tex()
{
    if (is_tex_loaded) return;
    is_tex_loaded = true;
    tex = LoadTextureFromImage(im);
}

void DragonLib::DImage::unload_tex()
{
    if (!is_tex_loaded) return;
    is_tex_loaded = false;
    UnloadTexture(tex);
}
