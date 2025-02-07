#include <stdexcept>
#include <enet_include.hpp>

#include "misc/sig_handler.hpp"
#include "lobby_service_provider/lobby_service_provider.hpp"

int main(int argc, char* argv[]){
    if (enet_initialize()){
        throw std::runtime_error("Could not initialize enet!");
    }
    SignalHandler::initialize();
    
    {
        LobbyServiceProvider server(8000);
        SignalHandler::ctrl_c_callback = [&server](){
            server.stop();
        };
        
        server.start();
    }

    enet_deinitialize();
}