#include "lobby_server.hpp"
#include <enet/enet.h>
#include <stdexcept>

int main(int argc, char const *argv[])
{
    if (enet_initialize()){
        throw std::runtime_error("Coudl not initialize enet!");
    }

    LobbyServer server;
    server.bind("127.0.0.1", 3000);
    server.run();
    
    server.cleanup();
    enet_deinitialize();
}
