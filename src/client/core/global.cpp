#include "global.hpp"
#include "core/scamlib/text.hpp"

namespace Global
{
    namespace Rendering
    {
        namespace Shaders
        {
            Shader sdf_font_shader;
        } // namespace Shaders

        namespace Fonts
        {
            Font main_sdf;
            Font main;
        } // namespace Fonts
    } // namespace Rendering
}

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