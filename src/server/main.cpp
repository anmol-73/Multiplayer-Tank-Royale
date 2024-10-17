#include "room_host.hpp"
#include <enet/enet.h>
#include <stdexcept>

int main(int argc, char const *argv[])
{
    if (enet_initialize()){
        throw std::runtime_error("Could not initialize enet!");
    }
    RoomHost server;
    server.bind(3000);
    
    server.run();
    
    server.cleanup();
    enet_deinitialize();
}
