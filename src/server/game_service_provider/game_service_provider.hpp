#ifndef H_GAME_SERVICE_PROVIDER
#define H_GAME_SERVICE_PROVIDER

#include "../service_provider.hpp"
#include "communication/protocol.hpp"
#include "misc/task.hpp"

struct GameServiceProvider: public ServiceProvider{
public:
    GameServiceProvider(std::string name, std::function<void(int)> destroy_callback): ServiceProvider(0), name(name), destroy_callback(destroy_callback) {};
    void start_async();
    std::string name;  
    std::function<void(int)> destroy_callback;  
private:
    
    Utils::Task life;
};

#endif