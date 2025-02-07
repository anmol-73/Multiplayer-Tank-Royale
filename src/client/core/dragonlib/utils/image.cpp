#include "image.hpp"

DragonLib::DImage::DImage(const std::string &path_)
{
    path = path_;
}

DragonLib::DImage::DImage() = default;

void DragonLib::DImage::load_im()
{
    im = LoadImage(path.c_str());
}

void DragonLib::DImage::unload_im()
{
    UnloadImage(im);
}

void DragonLib::DImage::load_tex()
{
    tex = LoadTextureFromImage(im);
}

void DragonLib::DImage::unload_tex()
{
    UnloadTexture(tex);
}
