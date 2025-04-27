#include "global.hpp"

Shader Global::Rendering::Shaders::sdf_font_shader{};
Shader Global::Rendering::Shaders::rendering_shader{};
Font Global::Rendering::Fonts::main{};
Font Global::Rendering::Fonts::main_sdf{};
float Global::rem{};

void Global::init()
{
    Rendering::Shaders::sdf_font_shader = LoadShader(0, TextFormat("resources/shaders/sdf.fs"));
    Rendering::Shaders::rendering_shader = LoadShader(0, TextFormat("resources/shaders/postproc.fs"));
    Rendering::Fonts::main_sdf = DragonLib::Utils::load_sdf("resources/exo2.ttf");
    Rendering::Fonts::main = GetFontDefault();

    Global::rem = GetScreenHeight() / 30;
}

void Global::cleanup()
{
    UnloadShader(Rendering::Shaders::sdf_font_shader);
    UnloadShader(Rendering::Shaders::rendering_shader);
    UnloadFont(Rendering::Fonts::main_sdf);
}
