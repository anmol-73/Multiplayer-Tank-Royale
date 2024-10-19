#include "room_host.hpp"
#include <stdexcept>
#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>
#include "sig_handler.hpp"

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
    #undef min
#endif



int main(int argc, char const *argv[])
{
    if (enet_initialize()){
        throw std::runtime_error("Could not initialize enet!");
    }
    SignalHandler::initialize();

    RoomHost server;
    SignalHandler::ctrlC_callback = [&server](){server.stop();};
    
    server.bind(3000);
    
    server.run();
    
    server.cleanup();
    enet_deinitialize();
}
