#include "game_service_provider.hpp"

void GameServiceProvider::start_async()
{
    if (life.is_running()){
        throw std::runtime_error("Service provider was asked to start twice!");
    }
    life.accomplish([this](bool _cancelled){
        start();
    });
}