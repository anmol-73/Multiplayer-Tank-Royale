#include "global.hpp"
#include "core/scamlib/text.hpp"

void Global::init()
{
    Rendering::Shaders::sdf_font_shader = LoadShader(0, TextFormat("resources/shaders/sdf.fs"));
    Rendering::Fonts::main_sdf = Scamlib::Load::font_sdf("resources/exo2.ttf");
    Rendering::Fonts::main = GetFontDefault();
}

void Global::free()
{
    UnloadShader(Rendering::Shaders::sdf_font_shader);
    UnloadFont(Rendering::Fonts::main_sdf);
}