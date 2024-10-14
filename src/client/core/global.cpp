#include "global.hpp"

Shader Global::Rendering::Shaders::sdf_font_shader{};
Font Global::Rendering::Fonts::main{};
Font Global::Rendering::Fonts::main_sdf{};

void Global::init()
{
    Rendering::Shaders::sdf_font_shader = LoadShader(0, TextFormat("resources/shaders/sdf.fs"));
    Rendering::Fonts::main_sdf = DragonLib::Utils::load_sdf("resources/exo2.ttf");
    Rendering::Fonts::main = GetFontDefault();
}

void Global::cleanup()
{
    UnloadShader(Rendering::Shaders::sdf_font_shader);
    UnloadFont(Rendering::Fonts::main_sdf);
}
