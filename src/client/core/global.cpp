#include "global.hpp"

Shader Global::Rendering::Shaders::sdf_font_shader{};
Font Global::Rendering::Fonts::main{};
Font Global::Rendering::Fonts::main_sdf{};
Communication::RoomClient Global::ServiceProviders::room_client{};
Utils::Task Global::ServiceProviders::room_client_worker{};
float Global::rem{};
std::vector<std::string> Global::names;

void Global::init()
{
    Rendering::Shaders::sdf_font_shader = LoadShader(0, TextFormat("resources/shaders/sdf.fs"));
    Rendering::Fonts::main_sdf = DragonLib::Utils::load_sdf("resources/exo2.ttf");
    Rendering::Fonts::main = GetFontDefault();

    Global::rem = GetScreenHeight() / 30;

    if (enet_initialize()){
        throw std::runtime_error("Could not initialize enet!");
    }
}

void Global::cleanup()
{
    UnloadShader(Rendering::Shaders::sdf_font_shader);
    UnloadFont(Rendering::Fonts::main_sdf);

    ServiceProviders::room_client.stop();
    ServiceProviders::room_client_worker.await();

    enet_deinitialize();
}
